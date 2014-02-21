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

#ifndef EOSOGRE_EOSODEDEBUGOBJECTS_H
#define EOSOGRE_EOSODEDEBUGOBJECTS_H

#include "Ogre.h"

// -----------------------------------------------------------

typedef std::vector<Ogre::Vector3> Vector3Array;

// -----------------------------------------------------------

class EOSOdeDebugLines : public Ogre::SimpleRenderable
	{
	public:
		EOSOdeDebugLines(void);
		~EOSOdeDebugLines(void);

		void addLine(const Ogre::Vector3 &start, const Ogre::Vector3 &end)
			{
			clear();
			_points.push_back(start);
			_points.push_back(end);
			}

		void addLine(Ogre::Real start_x, Ogre::Real start_y, Ogre::Real start_z, Ogre::Real end_x, Ogre::Real end_y, Ogre::Real end_z)
			{
			addLine(Ogre::Vector3(start_x, start_y, start_z), Ogre::Vector3(end_x, end_y, end_z));
			}

		void draw();
		void clear();

		Ogre::Real getSquaredViewDepth(const Ogre::Camera *cam) const;
		Ogre::Real getBoundingRadius(void) const;

	protected:
		Vector3Array _points;
		bool _drawn;

		static bool _materials_created;
	};


// -----------------------------------------------------------


class EOSOdeDebugObject : public EOSOdeDebugLines
	{
	public:
		enum Mode
			{
			Mode_Unknown,
			Mode_Enabled,
			Mode_Disabled,
			Mode_Static
			};

	public:
		EOSOdeDebugObject(EOSOdeDebugObject::Mode mode = EOSOdeDebugObject::Mode_Enabled);
		virtual ~EOSOdeDebugObject();

		void setMode(EOSOdeDebugObject::Mode mode);

	protected:
		EOSOdeDebugObject::Mode _mode;
	};

// -----------------------------------------------------------


class EOSOdeBoxDebugObject : public EOSOdeDebugObject
	{
	public:
		EOSOdeBoxDebugObject(const Ogre::Vector3& size);
		virtual ~EOSOdeBoxDebugObject();
	};


// -----------------------------------------------------------


class EOSOdeSphereDebugObject : public EOSOdeDebugObject
	{
	public:
		EOSOdeSphereDebugObject(Ogre::Real radius);
		virtual ~EOSOdeSphereDebugObject();
	};


// -----------------------------------------------------------


class EOSOdeCapsuleDebugObject : public EOSOdeDebugObject
	{
	public:
		EOSOdeCapsuleDebugObject(Ogre::Real radius, Ogre::Real length);
		virtual ~EOSOdeCapsuleDebugObject();
	};


// -----------------------------------------------------------


class EOSOdeTrimeshDebugObject : public EOSOdeDebugObject
	{
	public:
		EOSOdeTrimeshDebugObject(int vertex_count);
		virtual ~EOSOdeTrimeshDebugObject();

		void beginDefinition();
		void setVertex(int index, const Ogre::Vector3& vertex);
		void endDefinition();
	};


// -----------------------------------------------------------


class EOSOdeRayDebugObject : public EOSOdeDebugObject
	{
	public:
		EOSOdeRayDebugObject(const Ogre::Vector3& start, const Ogre::Vector3& direction, Ogre::Real length);
		virtual ~EOSOdeRayDebugObject();
	};


// -----------------------------------------------------------

#endif
