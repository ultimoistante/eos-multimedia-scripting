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


#include "EOSOdeDebugObjects.h"


// -----------------------------------------------------------------------------

bool EOSOdeDebugLines::_materials_created = false;

// -----------------------------------------------------------------------------

EOSOdeDebugLines::EOSOdeDebugLines() : Ogre::SimpleRenderable()
	{
	mRenderOp.vertexData = new Ogre::VertexData();
	_drawn = false;

	if (!_materials_created)
		{
		Ogre::MaterialPtr red   = Ogre::MaterialManager::getSingleton().create("EOSOdeDebugObject/Disabled", "EOSOde");
		Ogre::MaterialPtr green = Ogre::MaterialManager::getSingleton().create("EOSOdeDebugObject/Enabled",  "EOSOde");
		Ogre::MaterialPtr blue  = Ogre::MaterialManager::getSingleton().create("EOSOdeDebugObject/Static",   "EOSOde");

		red->setReceiveShadows(false);
		red->getTechnique(0)->setLightingEnabled(true);
		red->getTechnique(0)->getPass(0)->setSelfIllumination(1,0,0);

		green->setReceiveShadows(false);
		green->getTechnique(0)->setLightingEnabled(true);
		green->getTechnique(0)->getPass(0)->setSelfIllumination(0,1,0);

		blue->setReceiveShadows(false);
		blue->getTechnique(0)->setLightingEnabled(true);
		blue->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1);

		_materials_created = true;
		}
	setCastShadows (false);
	this->setMaterial("EOSOdeDebugObject/Enabled");
	}

// -----------------------------------------------------------------------------

void EOSOdeDebugLines::clear()
	{
	if(_drawn)
		{
		_drawn = false;
		_points.clear();
		delete mRenderOp.vertexData;

		mRenderOp.vertexData = new Ogre::VertexData();
		}
	}

// -----------------------------------------------------------------------------

EOSOdeDebugLines::~EOSOdeDebugLines(void)
	{
   clear();
   delete mRenderOp.vertexData;
	}

// -----------------------------------------------------------------------------

void EOSOdeDebugLines::draw()
	{
   if(_drawn)
		return;
   else
		_drawn = true;

   // Initialization stuff
   mRenderOp.indexData = 0;
   mRenderOp.vertexData->vertexCount = _points.size();
   mRenderOp.vertexData->vertexStart = 0;
   mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST;
   mRenderOp.useIndexes = false;

   Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
   Ogre::VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

   decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

   Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		decl->getVertexSize(0),
		mRenderOp.vertexData->vertexCount,
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

   bind->setBinding(0, vbuf);

   // Drawing stuff
   unsigned int size = (unsigned int)_points.size();
   Ogre::Vector3 vaabMin = _points[0];
   Ogre::Vector3 vaabMax = _points[0];

   float *prPos = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

   for(unsigned int i = 0; i < size; i++)
		{
      *prPos++ = _points[i].x;
      *prPos++ = _points[i].y;
      *prPos++ = _points[i].z;

      if (_points[i].x < vaabMin.x)
			vaabMin.x = _points[i].x;
      if (_points[i].y < vaabMin.y)
			vaabMin.y = _points[i].y;
      if (_points[i].z < vaabMin.z)
			vaabMin.z = _points[i].z;

      if (_points[i].x > vaabMax.x)
			vaabMax.x = _points[i].x;
      if (_points[i].y > vaabMax.y)
			vaabMax.y = _points[i].y;
      if (_points[i].z > vaabMax.z)
			vaabMax.z = _points[i].z;
		}

   vbuf->unlock();

   mBox.setExtents(vaabMin, vaabMax);
	}

// -----------------------------------------------------------------------------

Ogre::Real EOSOdeDebugLines::getSquaredViewDepth(const Ogre::Camera *cam) const
	{
	Ogre::Vector3 vMin, vMax, vMid, vDist;
	vMin = mBox.getMinimum();
	vMax = mBox.getMaximum();
	vMid = ((vMin - vMax) * 0.5) + vMin;
	vDist = cam->getDerivedPosition() - vMid;

	return vDist.squaredLength();
	}

// -----------------------------------------------------------------------------

Ogre::Real EOSOdeDebugLines::getBoundingRadius() const
	{
	return Ogre::Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}

// -----------------------------------------------------------------------------

EOSOdeDebugObject::EOSOdeDebugObject(EOSOdeDebugObject::Mode mode)
	{
	_mode = EOSOdeDebugObject::Mode_Unknown;
	setMode(mode);
	}

// -----------------------------------------------------------------------------

void EOSOdeDebugObject::setMode(EOSOdeDebugObject::Mode mode)
	{
	if(mode != _mode)
		{
		_mode = mode;
		switch(_mode)
			{
			case EOSOdeDebugObject::Mode_Enabled:
				setMaterial("EOSOdeDebugObject/Enabled");
				break;
			case EOSOdeDebugObject::Mode_Disabled:
				setMaterial("EOSOdeDebugObject/Disabled");
				break;
			case EOSOdeDebugObject::Mode_Static:
				setMaterial("EOSOdeDebugObject/Static");
				break;
			}
		}
	}

// -----------------------------------------------------------------------------

EOSOdeDebugObject::~EOSOdeDebugObject()
	{
	}

// -----------------------------------------------------------------------------

EOSOdeBoxDebugObject::EOSOdeBoxDebugObject(const Ogre::Vector3& size) : EOSOdeDebugObject()
	{
	Ogre::AxisAlignedBox aabb(-size.x * 0.5, -size.y * 0.5, -size.z * 0.5, size.x * 0.5, size.y * 0.5, size.z * 0.5);

	Ogre::Vector3 vmax = aabb.getMaximum();
	Ogre::Vector3 vmin = aabb.getMinimum();

	addLine(vmin.x,vmin.y,vmin.z,vmax.x,vmin.y,vmin.z);
	addLine(vmin.x,vmin.y,vmin.z,vmin.x,vmin.y,vmax.z);
	addLine(vmin.x,vmin.y,vmin.z,vmin.x,vmax.y,vmin.z);
	addLine(vmin.x,vmax.y,vmin.z,vmin.x,vmax.y,vmax.z);
	addLine(vmin.x,vmax.y,vmin.z,vmax.x,vmax.y,vmin.z);
	addLine(vmax.x,vmin.y,vmin.z,vmax.x,vmin.y,vmax.z);
	addLine(vmax.x,vmin.y,vmin.z,vmax.x,vmax.y,vmin.z);
	addLine(vmin.x,vmax.y,vmax.z,vmax.x,vmax.y,vmax.z);
	addLine(vmin.x,vmax.y,vmax.z,vmin.x,vmin.y,vmax.z);
	addLine(vmax.x,vmax.y,vmin.z,vmax.x,vmax.y,vmax.z);
	addLine(vmax.x,vmin.y,vmax.z,vmax.x,vmax.y,vmax.z);
	addLine(vmin.x,vmin.y,vmax.z,vmax.x,vmin.y,vmax.z);

	draw();
	}

// -----------------------------------------------------------------------------

EOSOdeBoxDebugObject::~EOSOdeBoxDebugObject()
	{
	}

// -----------------------------------------------------------------------------

EOSOdeSphereDebugObject::EOSOdeSphereDebugObject(Ogre::Real radius) : EOSOdeDebugObject()
	{
	// X/Y axis

	// NW quadrant
	addLine(-radius,			0.0,				0.0,
	-0.866 * radius,	0.5 * radius,		0.0);

	addLine(-0.866 * radius,	0.5 * radius,		0.0,
	-0.5 * radius,	0.866 * radius,	0.0);

	addLine(-0.5 * radius,	0.866 * radius,	0.0,
	0.0,				radius,			0.0);

	// NE quadrant
	addLine(0.0,				radius,			0.0,
	0.5 * radius,		0.866 * radius,	0.0);

	addLine(0.5 * radius,		0.866 * radius,	0.0,
	0.866 * radius,	0.5 * radius,		0.0);

	addLine(0.866 * radius,	0.5 * radius,		0.0,
	radius,			0.0,				0.0);

	// SW quadrant
	addLine(-radius,			0.0,				0.0,
	-0.866 * radius,	-0.5 * radius,	0.0);

	addLine(-0.866 * radius,	-0.5 * radius,	0.0,
	-0.5 * radius,	-0.866 * radius,	0.0);

	addLine(-0.5 * radius,	-0.866 * radius,	0.0,
	0.0,				-radius,			0.0);

	// SE quadrant
	addLine(0.0,				-radius,			0.0,
	0.5 * radius,		-0.866 * radius,	0.0);

	addLine(0.5 * radius,		-0.866 * radius,	0.0,
	0.866 * radius,	-0.5 * radius,	0.0);

	addLine(0.866 * radius,	-0.5 * radius,	0.0,
	radius,			0.0,				0.0);

	// X/Z axis

	// NW quadrant
	addLine(-radius,			0.0,	0.0,
	-0.866 * radius,	0.0,	0.5 * radius);

	addLine(-0.866 * radius,	0.0,	0.5 * radius,
	-0.5 * radius,	0.0,	0.866 * radius);

	addLine(-0.5 * radius,	0.0,	0.866 * radius,
	0.0,				0.0,	radius);

	// NE quadrant
	addLine(0.0,				0.0,	radius,
	0.5 * radius,		0.0,	0.866 * radius);

	addLine(0.5 * radius,		0.0,	0.866 * radius,
	0.866 * radius,	0.0,	0.5 * radius);

	addLine(0.866 * radius,	0.0,	0.5 * radius,
	radius,			0.0,	0.0);

	// SW quadrant
	addLine(-radius,			0.0,	0.0,
	-0.866 * radius,	0.0,	-0.5 * radius);

	addLine(-0.866 * radius,	0.0,	-0.5 * radius,
	-0.5 * radius,	0.0,	-0.866 * radius);

	addLine(-0.5 * radius,	0.0,	-0.866 * radius,
	0.0,				0.0,	-radius);

	// SE quadrant
	addLine(0.0,				0.0,	-radius,
	0.5 * radius,		0.0,	-0.866 * radius);

	addLine(0.5 * radius,		0.0,	-0.866 * radius,
	0.866 * radius,	0.0,	-0.5 * radius);

	addLine(0.866 * radius,	0.0,	-0.5 * radius,
	radius,			0.0,	0.0);

	// Y/Z axis

	// NW quadrant
	addLine(0.0,	-radius,			0.0,
	0.0,	-0.866 * radius,	0.5 * radius);

	addLine(0.0,	-0.866 * radius,	0.5 * radius,
	0.0,	-0.5 * radius,	0.866 * radius);

	addLine(0.0,	-0.5 * radius,	0.866 * radius,
	0.0,	0.0,				radius);

	// NE quadrant
	addLine(0.0,	0.0,				radius,
	0.0,	0.5 * radius,		0.866 * radius);

	addLine(0.0,	0.5 * radius,		0.866 * radius,
	0.0,	0.866 * radius,	0.5 * radius);

	addLine(0.0,	0.866 * radius,	0.5 * radius,
	0.0,	radius,			0.0);

	// SW quadrant
	addLine(0.0,	-radius,			0.0,
	0.0,	-0.866 * radius,	-0.5 * radius);

	addLine(0.0,	-0.866 * radius,	-0.5 * radius,
	0.0,	-0.5 * radius,	-0.866 * radius);

	addLine(0.0,	-0.5 * radius,	-0.866 * radius,
	0.0,	0.0,				-radius);

	// SE quadrant
	addLine(0.0,	0.0,				-radius,
	0.0,	0.5 * radius,		-0.866 * radius);

	addLine(0.0,	0.5 * radius,		-0.866 * radius,
	0.0,	0.866 * radius,	-0.5 * radius);

	addLine(0.0,	0.866 * radius,	-0.5 * radius,
	0.0,	radius,			0.0);

	draw();
	}

// -----------------------------------------------------------------------------

EOSOdeSphereDebugObject::~EOSOdeSphereDebugObject()
	{
	}

// -----------------------------------------------------------------------------

EOSOdeCapsuleDebugObject::EOSOdeCapsuleDebugObject(Ogre::Real radius, Ogre::Real length) : EOSOdeDebugObject()
	{
	Ogre::Real halflen = length * 0.5;
	Ogre::AxisAlignedBox aabb(-radius, -radius, -(halflen + radius), radius, radius, halflen + radius);

	// X/Y axis - Near

	// NW quadrant
	addLine(-radius,			0.0,				halflen,
	-0.866 * radius,	0.5 * radius,		halflen);

	addLine(-0.866 * radius,	0.5 * radius,		halflen,
		-0.5 * radius,	0.866 * radius,	halflen);

	addLine(-0.5 * radius,	0.866 * radius,	halflen,
	0.0,				radius,			halflen);

	// NE quadrant
	addLine(0.0,				radius,			halflen,
		0.5 * radius,		0.866 * radius,	halflen);

	addLine(0.5 * radius,		0.866 * radius,	halflen,
	0.866 * radius,	0.5 * radius,		halflen);

	addLine(0.866 * radius,	0.5 * radius,		halflen,
	radius,			0.0,				halflen);

	// SW quadrant
	addLine(-radius,			0.0,				halflen,
	-0.866 * radius,	-0.5 * radius,	halflen);

	addLine(-0.866 * radius,	-0.5 * radius,	halflen,
	-0.5 * radius,	-0.866 * radius,	halflen);

	addLine(-0.5 * radius,	-0.866 * radius,	halflen,
	0.0,				-radius,			halflen);

	// SE quadrant
	addLine(0.0,				-radius,			halflen,
	0.5 * radius,		-0.866 * radius,	halflen);

	addLine(0.5 * radius,		-0.866 * radius,	halflen,
	0.866 * radius,	-0.5 * radius,	halflen);

	addLine(0.866 * radius,	-0.5 * radius,	halflen,
	radius,			0.0,				halflen);

	// X/Y axis - Far

	// NW quadrant
	addLine(-radius,			0.0,				-halflen,
	-0.866 * radius,	0.5 * radius,		-halflen);

	addLine(-0.866 * radius,	0.5 * radius,		-halflen,
	-0.5 * radius,	0.866 * radius,	-halflen);

	addLine(-0.5 * radius,	0.866 * radius,	-halflen,
	0.0,				radius,			-halflen);

	// NE quadrant
	addLine(0.0,				radius,			-halflen,
	0.5 * radius,		0.866 * radius,	-halflen);

	addLine(0.5 * radius,		0.866 * radius,	-halflen,
	0.866 * radius,	0.5 * radius,		-halflen);

	addLine(0.866 * radius,	0.5 * radius,		-halflen,
	radius,			0.0,				-halflen);

	// SW quadrant
	addLine(-radius,			0.0,				-halflen,
	-0.866 * radius,	-0.5 * radius,	-halflen);

	addLine(-0.866 * radius,	-0.5 * radius,	-halflen,
	-0.5 * radius,	-0.866 * radius,	-halflen);

	addLine(-0.5 * radius,	-0.866 * radius,	-halflen,
	0.0,				-radius,			-halflen);

	// SE quadrant
	addLine(0.0,				-radius,			-halflen,
	0.5 * radius,		-0.866 * radius,	-halflen);

	addLine(0.5 * radius,		-0.866 * radius,	-halflen,
	0.866 * radius,	-0.5 * radius,	-halflen);

	addLine(0.866 * radius,	-0.5 * radius,	-halflen,
	radius,			0.0,				-halflen);

	// X/Z axis

	// NW quadrant
	addLine(-radius,			0.0,	halflen,
	-0.866 * radius,	0.0,	(0.5 * radius) + halflen);

	addLine(-0.866 * radius,	0.0,	(0.5 * radius) + halflen,
	-0.5 * radius,	0.0,	(0.866 * radius) + halflen);

	addLine(-0.5 * radius,	0.0,	(0.866 * radius) + halflen,
	0.0,				0.0,	radius + halflen);

	// NE quadrant
	addLine(0.0,				0.0,	radius + halflen,
	0.5 * radius,		0.0,	(0.866 * radius) + halflen);

	addLine(0.5 * radius,		0.0,	(0.866 * radius) + halflen,
	0.866 * radius,	0.0,	(0.5 * radius) + halflen);

	addLine(0.866 * radius,	0.0,	(0.5 * radius) + halflen,
	radius,			0.0,	halflen);

	// SW quadrant
	addLine(-radius,			0.0,	-halflen,
	-0.866 * radius,	0.0,	(-0.5 * radius) - halflen);

	addLine(-0.866 * radius,	0.0,	(-0.5 * radius) - halflen,
	-0.5 * radius,	0.0,	(-0.866 * radius) - halflen);

	addLine(-0.5 * radius,	0.0,	(-0.866 * radius) - halflen,
	0.0,				0.0,	-radius - halflen);

	// SE quadrant
	addLine(0.0,				0.0,	-radius - halflen,
	0.5 * radius,		0.0,	(-0.866 * radius) - halflen);

	addLine(0.5 * radius,		0.0,	(-0.866 * radius) - halflen,
	0.866 * radius,	0.0,	(-0.5 * radius) - halflen);

	addLine(0.866 * radius,	0.0,	(-0.5 * radius) - halflen,
	radius,			0.0,	- halflen);

	// Y/Z axis

	// NW quadrant
	addLine(0.0,	-radius,			halflen,
	0.0,	-0.866 * radius,	(0.5 * radius) + halflen);

	addLine(0.0,	-0.866 * radius,	(0.5 * radius) + halflen,
	0.0,	-0.5 * radius,	(0.866 * radius) + halflen);

	addLine(0.0,	-0.5 * radius,	(0.866 * radius) + halflen,
	0.0,	0.0,				radius + halflen);

	// NE quadrant
	addLine(0.0,	0.0,				radius + halflen,
	0.0,	0.5 * radius,		(0.866 * radius) + halflen);

	addLine(0.0,	0.5 * radius,		(0.866 * radius) + halflen,
	0.0,	0.866 * radius,	(0.5 * radius) + halflen);

	addLine(0.0,	0.866 * radius,	(0.5 * radius) + halflen,
	0.0,	radius,			halflen);

	// SW quadrant
	addLine(0.0,	-radius,			-halflen,
	0.0,	-0.866 * radius,	(-0.5 * radius) - halflen);

	addLine(0.0,	-0.866 * radius,	(-0.5 * radius) - halflen,
	0.0,	-0.5 * radius,	(-0.866 * radius) - halflen);

	addLine(0.0,	-0.5 * radius,	(-0.866 * radius) - halflen,
	0.0,	0.0,				-radius - halflen);

	// SE quadrant
	addLine(0.0,	0.0,				-radius - halflen,
	0.0,	0.5 * radius,		(-0.866 * radius) - halflen);

	addLine(0.0,	0.5 * radius,		(-0.866 * radius) - halflen,
	0.0,	0.866 * radius,	(-0.5 * radius) - halflen);

	addLine(0.0,	0.866 * radius,	(-0.5 * radius) - halflen,
	0.0,	radius,			-halflen);

	// Side lines
	addLine(-radius,	0.0,		-halflen,
	-radius,	0.0,		halflen);

	addLine(radius,	0.0,		-halflen,
	radius,	0.0,		halflen);

	addLine(0.0,		radius,	-halflen,
	0.0,		radius,	halflen);

	addLine(0.0,		-radius,	-halflen,
	0.0,		-radius,	halflen);

	draw();
	}

// -----------------------------------------------------------------------------

EOSOdeCapsuleDebugObject::~EOSOdeCapsuleDebugObject()
	{
	}

// -----------------------------------------------------------------------------

EOSOdeTrimeshDebugObject::EOSOdeTrimeshDebugObject(int vertex_count) : EOSOdeDebugObject()
	{
	_points.reserve(vertex_count);
	_points.resize(vertex_count);
	}

// -----------------------------------------------------------------------------

void EOSOdeTrimeshDebugObject::beginDefinition()
	{
	}

// -----------------------------------------------------------------------------

void EOSOdeTrimeshDebugObject::setVertex(int index, const Ogre::Vector3& vertex)
	{
   assert ((unsigned int)index < _points.size());
	_points[index] = vertex;
	}

// -----------------------------------------------------------------------------

void EOSOdeTrimeshDebugObject::endDefinition()
	{
	draw();
	}

// -----------------------------------------------------------------------------

EOSOdeTrimeshDebugObject::~EOSOdeTrimeshDebugObject()
	{
	}

// -----------------------------------------------------------------------------

EOSOdeRayDebugObject::EOSOdeRayDebugObject(const Ogre::Vector3& start, const Ogre::Vector3& direction, Ogre::Real length) : EOSOdeDebugObject()
	{
	Ogre::Vector3 end = start + (direction.normalisedCopy() * length);
	addLine(start, end);
	draw();
	}

// -----------------------------------------------------------------------------

EOSOdeRayDebugObject::~EOSOdeRayDebugObject()
	{
	}

// -----------------------------------------------------------------------------
