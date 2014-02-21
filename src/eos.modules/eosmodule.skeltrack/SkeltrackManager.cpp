/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                           (mailto: ultimoistante@gmail.com | www.startupsolutions.it)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/


#include "SkeltrackManager.h"

// -----------------------------------------------------------------------------

bool SkeltrackManager::instanceFlag = false;
SkeltrackManager* SkeltrackManager::instance = NULL;

// -----------------------------------------------------------------------------

static SkeltrackSkeleton *skeleton = NULL;
static GFreenectDevice *kinect = NULL;
static ClutterActor *info_text;
static ClutterActor *depth_tex;
static ClutterActor *video_tex;
static SkeltrackJointList list = NULL;

static gboolean SHOW_SKELETON = TRUE;
static gboolean ENABLE_SMOOTHING = TRUE;
static gfloat SMOOTHING_FACTOR = .0;

static guint THRESHOLD_BEGIN = 500;
/* Adjust this value to increase of decrease
   the threshold */
static guint THRESHOLD_END   = 1500;


typedef struct
    {
    guint16 *reduced_buffer;
    gint width;
    gint height;
    gint reduced_width;
    gint reduced_height;
    } BufferInfo;


static void on_track_joints (GObject* obj, GAsyncResult *res, gpointer user_data)
    {
    guint i;
    BufferInfo *buffer_info;
    guint16 *reduced;
    gint width, height, reduced_width, reduced_height;
    GError *error = NULL;

    buffer_info = (BufferInfo *) user_data;
    reduced = (guint16 *) buffer_info->reduced_buffer;
    width = buffer_info->width;
    height = buffer_info->height;
    reduced_width = buffer_info->reduced_width;
    reduced_height = buffer_info->reduced_height;

    list = skeltrack_skeleton_track_joints_finish (skeleton, res, &error);

    if (error == NULL)
        {
        if (SHOW_SKELETON)
            clutter_cairo_texture_invalidate (CLUTTER_CAIRO_TEXTURE (depth_tex));
        }
    else
        {
        g_warning ("%s\n", error->message);
        g_error_free (error);
        }

    g_slice_free1 (reduced_width * reduced_height * sizeof (guint16), reduced);

    g_slice_free (BufferInfo, buffer_info);

    skeltrack_joint_list_free (list);
    }


static void grayscale_buffer_set_value (guchar *buffer, gint index, guchar value)
    {
    buffer[index * 3] = value;
    buffer[index * 3 + 1] = value;
    buffer[index * 3 + 2] = value;
    }


static BufferInfo* process_buffer (guint16 *buffer, guint width, guint height, guint dimension_factor, guint threshold_begin, guint threshold_end)
    {
    BufferInfo *buffer_info;
    gint i, j, reduced_width, reduced_height;
    guint16 *reduced_buffer;

    g_return_val_if_fail (buffer != NULL, NULL);

    reduced_width = (width - width % dimension_factor) / dimension_factor;
    reduced_height = (height - height % dimension_factor) / dimension_factor;

    reduced_buffer = (guint16*) g_slice_alloc0 (reduced_width * reduced_height * sizeof (guint16));

    for (i = 0; i < reduced_width; i++)
        {
        for (j = 0; j < reduced_height; j++)
            {
            gint index;
            guint16 value;

            index = j * width * dimension_factor + i * dimension_factor;
            value = buffer[index];

            if (value < threshold_begin || value > threshold_end)
                {
                reduced_buffer[j * reduced_width + i] = 0;
                continue;
                }

            reduced_buffer[j * reduced_width + i] = value;
            }
        }

    buffer_info = g_slice_new0 (BufferInfo);
    buffer_info->reduced_buffer = reduced_buffer;
    buffer_info->reduced_width = reduced_width;
    buffer_info->reduced_height = reduced_height;
    buffer_info->width = width;
    buffer_info->height = height;

    return buffer_info;
    }


static guchar* create_grayscale_buffer (BufferInfo *buffer_info, gint dimension_reduction)
    {
    gint i,j;
    gint size;
    guchar *grayscale_buffer;
    guint16 *reduced_buffer;

    reduced_buffer = buffer_info->reduced_buffer;

    size = buffer_info->width * buffer_info->height * sizeof (guchar) * 3;
    grayscale_buffer = (guchar*)g_slice_alloc (size);
    /*Paint is white*/
    memset (grayscale_buffer, 255, size);

    for (i = 0; i < buffer_info->reduced_width; i++)
        {
        for (j = 0; j < buffer_info->reduced_height; j++)
            {
            if (reduced_buffer[j * buffer_info->reduced_width + i] != 0)
                {
                gint index = j * dimension_reduction * buffer_info->width +
                             i * dimension_reduction;
                grayscale_buffer_set_value (grayscale_buffer, index, 0);
                }
            }
        }

    return grayscale_buffer;
    }


static void on_depth_frame (GFreenectDevice *kinect, gpointer user_data)
    {
    gboolean smoothing_enabled;
    gint width, height;
    gint dimension_factor;
    guchar *grayscale_buffer;
    guint16 *depth;
    BufferInfo *buffer_info;
    gsize len;
    GError *error = NULL;
    GFreenectFrameMode frame_mode;

    depth = (guint16 *) gfreenect_device_get_depth_frame_raw (kinect, &len, &frame_mode);

    width = frame_mode.width;
    height = frame_mode.height;

    g_object_get (skeleton, "dimension-reduction", &dimension_factor, NULL);

    buffer_info = process_buffer (depth, width, height, dimension_factor, THRESHOLD_BEGIN, THRESHOLD_END);

    skeltrack_skeleton_track_joints (skeleton, buffer_info->reduced_buffer, buffer_info->reduced_width, buffer_info->reduced_height, NULL, on_track_joints, buffer_info);

    if (!SHOW_SKELETON)
        {
        grayscale_buffer = create_grayscale_buffer (buffer_info, dimension_factor);
        if (! clutter_texture_set_from_rgb_data (CLUTTER_TEXTURE (depth_tex), grayscale_buffer, FALSE, width, height, 0, 3, CLUTTER_TEXTURE_NONE, &error))
            {
            g_debug ("Error setting texture area: %s", error->message);
            g_error_free (error);
            }
        g_slice_free1 (width * height * sizeof (guchar) * 3, grayscale_buffer);
        }
    }


static void on_video_frame (GFreenectDevice *kinect, gpointer user_data)
    {
    guchar *buffer;
    GError *error = NULL;
    GFreenectFrameMode frame_mode;

    buffer = gfreenect_device_get_video_frame_rgb (kinect, NULL, &frame_mode);

    if (! clutter_texture_set_from_rgb_data (CLUTTER_TEXTURE (video_tex), buffer, FALSE, frame_mode.width, frame_mode.height, 0, frame_mode.bits_per_pixel / 8, CLUTTER_TEXTURE_NONE, &error))
        {
        g_debug ("Error setting texture area: %s", error->message);
        g_error_free (error);
        }
    }


/*static void paint_joint (cairo_t *cairo, SkeltrackJoint *joint, gint radius, const gchar *color_str)
    {
    ClutterColor *color;

    if (joint == NULL)
        return;

    color = clutter_color_new (0, 0, 0, 200);
    clutter_color_from_string (color, color_str);

    cairo_set_line_width (cairo, 10);
    clutter_cairo_set_source_color (cairo, color);
    cairo_arc (cairo,
               joint->screen_x,
               joint->screen_y,
               radius * (THRESHOLD_END - THRESHOLD_BEGIN) / joint->z,
               0,
               G_PI * 2);
    cairo_fill (cairo);
    clutter_color_free (color);
    }*/


/*static void connect_joints (cairo_t *cairo, SkeltrackJoint *joint_a, SkeltrackJoint *joint_b, const gchar *color_str)
    {
    ClutterColor *color;

    if (joint_a == NULL || joint_b == NULL)
        return;

    color = clutter_color_new (0, 0, 0, 200);
    clutter_color_from_string (color, color_str);

    cairo_set_line_width (cairo, 10);
    clutter_cairo_set_source_color (cairo, color);
    cairo_move_to (cairo, joint_a->screen_x, joint_a->screen_y);
    cairo_line_to (cairo, joint_b->screen_x, joint_b->screen_y);
    cairo_stroke (cairo);
    clutter_color_free (color);
    }*/


static void on_texture_draw (ClutterCairoTexture *texture, cairo_t *cairo, gpointer user_data)
    {
    //printf("on_texture_draw\n");
    guint width, height;
    ClutterColor *color;
    SkeltrackJoint *head, *left_hand, *right_hand, *left_shoulder, *right_shoulder, *left_elbow, *right_elbow;

    if (list == NULL)
        return;

    head = skeltrack_joint_list_get_joint (list,
                                           SKELTRACK_JOINT_ID_HEAD);
    left_hand = skeltrack_joint_list_get_joint (list,
                SKELTRACK_JOINT_ID_LEFT_HAND);
    right_hand = skeltrack_joint_list_get_joint (list,
                 SKELTRACK_JOINT_ID_RIGHT_HAND);
    left_shoulder = skeltrack_joint_list_get_joint (list,
                    SKELTRACK_JOINT_ID_LEFT_SHOULDER);
    right_shoulder = skeltrack_joint_list_get_joint (list,
                     SKELTRACK_JOINT_ID_RIGHT_SHOULDER);
    left_elbow = skeltrack_joint_list_get_joint (list,
                 SKELTRACK_JOINT_ID_LEFT_ELBOW);
    right_elbow = skeltrack_joint_list_get_joint (list,
                  SKELTRACK_JOINT_ID_RIGHT_ELBOW);

    
    if (head != NULL)
        printf("head: %d, %d\n", head->screen_x, head->screen_y);
    if (left_hand != NULL)
        printf("left_hand: %d, %d\n", left_hand->screen_x, left_hand->screen_y);
    if (right_hand != NULL)
        printf("right_hand: %d, %d\n", right_hand->screen_x, right_hand->screen_y);
    
    /*
    clutter_cairo_texture_clear (texture);
    clutter_cairo_texture_get_surface_size (texture, &width, &height);
    color = clutter_color_new (255, 255, 255, 255);
    clutter_cairo_set_source_color (cairo, color);
    cairo_rectangle (cairo, 0, 0, width, height);
    cairo_fill (cairo);
    clutter_color_free (color);

    paint_joint (cairo, head, 50, "#FFF800");

    connect_joints (cairo, left_shoulder, right_shoulder, "#afafaf");

    connect_joints (cairo, left_shoulder, left_elbow, "#afafaf");

    connect_joints (cairo, right_shoulder, right_elbow, "#afafaf");

    connect_joints (cairo, right_hand, right_elbow, "#afafaf");

    connect_joints (cairo, left_hand, left_elbow, "#afafaf");

    paint_joint (cairo, left_hand, 30, "#C2FF00");

    paint_joint (cairo, right_hand, 30, "#00FAFF");*/

    skeltrack_joint_list_free (list);
    list = NULL;
    }


/*static void set_info_text (void)
    {
    gchar *title;
    title = g_strdup_printf ("<b>Current View:</b> %s\t\t\t"
                             "<b>Threshold:</b> %d\n"
                             "<b>Smoothing Enabled:</b> %s\t\t\t"
                             "<b>Smoothing Level:</b> %.2f",
                             SHOW_SKELETON ? "Skeleton" : "Point Cloud",
                             THRESHOLD_END,
                             ENABLE_SMOOTHING ? "Yes" : "No",
                             SMOOTHING_FACTOR);
    clutter_text_set_markup (CLUTTER_TEXT (info_text), title);
    g_free (title);
    }*/


static void set_threshold (gint difference)
    {
    gint new_threshold = THRESHOLD_END + difference;
    if (new_threshold >= THRESHOLD_BEGIN + 300 &&
            new_threshold <= 4000)
        THRESHOLD_END = new_threshold;
    }


static void set_tilt_angle (GFreenectDevice *kinect, gdouble difference)
    {
    GError *error = NULL;
    gdouble angle;
    angle = gfreenect_device_get_tilt_angle_sync (kinect, NULL, &error);
    if (error != NULL)
        {
        g_error_free (error);
        return;
        }

    if (angle >= -31 && angle <= 31)
        gfreenect_device_set_tilt_angle (kinect,
                                         angle + difference,
                                         NULL,
                                         NULL,
                                         NULL);
    }


static void enable_smoothing (gboolean enable)
    {
    if (skeleton != NULL)
        g_object_set (skeleton, "enable-smoothing", enable, NULL);
    }


static void set_smoothing_factor (gfloat factor)
    {
    if (skeleton != NULL)
        {
        SMOOTHING_FACTOR += factor;
        SMOOTHING_FACTOR = CLAMP (SMOOTHING_FACTOR, 0.0, 1.0);
        g_object_set (skeleton, "smoothing-factor", SMOOTHING_FACTOR, NULL);
        }
    }


/*static gboolean on_key_release (ClutterActor *actor,
                ClutterEvent *event,
                gpointer data)
    {
    GFreenectDevice *kinect;
    gdouble angle;
    guint key;
    g_return_val_if_fail (event != NULL, FALSE);

    kinect = GFREENECT_DEVICE (data);

    key = clutter_event_get_key_symbol (event);
    switch (key)
        {
        case CLUTTER_KEY_space:
            SHOW_SKELETON = !SHOW_SKELETON;
            break;
        case CLUTTER_KEY_plus:
            set_threshold (100);
            break;
        case CLUTTER_KEY_minus:
            set_threshold (-100);
            break;
        case CLUTTER_KEY_Up:
            set_tilt_angle (kinect, 5);
            break;
        case CLUTTER_KEY_Down:
            set_tilt_angle (kinect, -5);
            break;
        case CLUTTER_KEY_s:
            ENABLE_SMOOTHING = !ENABLE_SMOOTHING;
            enable_smoothing (ENABLE_SMOOTHING);
            break;
        case CLUTTER_KEY_Right:
            set_smoothing_factor (.05);
            break;
        case CLUTTER_KEY_Left:
            set_smoothing_factor (-.05);
            break;
        }
    set_info_text ();
    return TRUE;
    }*/


/*static ClutterActor* create_instructions (void)
    {
    ClutterActor *text;

    text = clutter_text_new ();
    clutter_text_set_markup (CLUTTER_TEXT (text),
                             "<b>Instructions:</b>\n"
                             "\tChange between skeleton\n"
                             "\t  tracking and threshold view:  \tSpace bar\n"
                             "\tSet tilt angle:  \t\t\t\tUp/Down Arrows\n"
                             "\tIncrease threshold:  \t\t\t+/-\n"
                             "\tEnable/Disable smoothing:  \t\ts\n"
                             "\tSet smoothing level:  \t\t\tLeft/Right Arrows\n"
                            );
    return text;
    }*/


static void on_destroy (ClutterActor *actor, gpointer data)
    {
    GFreenectDevice *device = GFREENECT_DEVICE (data);
    gfreenect_device_stop_depth_stream (device, NULL);
    gfreenect_device_stop_video_stream (device, NULL);
    clutter_main_quit ();
    }


static void on_new_kinect_device (GObject* obj, GAsyncResult *res, gpointer user_data)
    {
    ClutterActor *stage, *instructions;
    GError *error = NULL;
    gint width = 640;
    gint height = 480;

    kinect = gfreenect_device_new_finish (res, &error);
    if (kinect == NULL)
        {
        g_debug ("Failed to created kinect device: %s", error->message);
        g_error_free (error);
        clutter_main_quit ();
        return;
        }

    g_debug ("Kinect device created!");

    /*stage = clutter_stage_get_default ();
    clutter_stage_set_title (CLUTTER_STAGE (stage), "Kinect Test");
    clutter_actor_set_size (stage, width * 2, height + 250);
    clutter_stage_set_user_resizable (CLUTTER_STAGE (stage), TRUE);

    g_signal_connect (stage, "destroy", G_CALLBACK (on_destroy), kinect);
    g_signal_connect (stage,
                      "key-release-event",
                      G_CALLBACK (on_key_release),
                      kinect);*/

    depth_tex = clutter_cairo_texture_new (width, height);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), depth_tex);

    video_tex = clutter_cairo_texture_new (width, height);
    clutter_actor_set_position (video_tex, width, 0.0);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), video_tex);

    /*info_text = clutter_text_new ();
    clutter_actor_set_position (info_text, 50, height + 20);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), info_text);*/

    /*instructions = create_instructions ();
    clutter_actor_set_position (instructions, 50, height + 70);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), instructions);*/

    //clutter_actor_show_all (stage);

    skeleton = skeltrack_skeleton_new ();
    g_object_get (skeleton, "smoothing-factor", &SMOOTHING_FACTOR, NULL);

    //set_info_text ();

    g_signal_connect (kinect,
                      "depth-frame",
                      G_CALLBACK (on_depth_frame),
                      NULL);

    g_signal_connect (kinect,
                      "video-frame",
                      G_CALLBACK (on_video_frame),
                      NULL);

    g_signal_connect (depth_tex,
                      "draw",
                      G_CALLBACK (on_texture_draw),
                      NULL);

    gfreenect_device_set_tilt_angle (kinect, 0, NULL, NULL, NULL);

    gfreenect_device_start_depth_stream (kinect,
                                         GFREENECT_DEPTH_FORMAT_MM,
                                         NULL);

    gfreenect_device_start_video_stream (kinect,
                                         GFREENECT_RESOLUTION_MEDIUM,
                                         GFREENECT_VIDEO_FORMAT_RGB, NULL);
    }


// --------------------------------------------------------------------

SkeltrackManager::SkeltrackManager()
    {
    printf("SkeltrackManager()\n");
    }

// --------------------------------------------------------------------

SkeltrackManager::~SkeltrackManager()
    {
    /* if (kinect != NULL)
            g_object_unref (kinect);

        if (skeleton != NULL)
            {
            g_object_unref (skeleton);
            }*/
    }

// -----------------------------------------------------------------------------

SkeltrackManager* SkeltrackManager::getInstance()
    {
    if (!instanceFlag)
        {
        instance = new SkeltrackManager();
        instanceFlag = true;
        }
    return instance;
    }

// -----------------------------------------------------------------------------

bool SkeltrackManager::powerOn()
    {
    g_type_init();

    if (clutter_init (0, NULL) != CLUTTER_INIT_SUCCESS)
        return false;

    gfreenect_device_new(0, GFREENECT_SUBDEVICE_CAMERA, NULL, on_new_kinect_device, NULL);
    //clutter_main ();

    return true;
    }

// -----------------------------------------------------------------------------

void SkeltrackManager::shutdown()
    {
    if (kinect != NULL)
        g_object_unref (kinect);

    if (skeleton != NULL)
        {
        g_object_unref (skeleton);
        }
    }


// -----------------------------------------------------------------------------

void SkeltrackManager::update()
    {
    g_main_context_iteration(NULL, true);
    }


// -----------------------------------------------------------------------------
