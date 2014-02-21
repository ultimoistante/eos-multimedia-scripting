/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                          (mailto: ultimoistante@gmail.com | www.startupsolutions.it)
                       and Alessandro Rasulo (mailto: alex9978@gmail.com)

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

#include "Box2dBody.h"
#include "Box2d.h"

// -----------------------------------------------------------------------------

Box2dBody::Box2dBody(float friction, float restitution, float density)
    {
    b2BodyDef bodyDef;
    bodyDef.position.Set(0.0f, 0.0f);
    //
    body = Box2d::getInstance()->getWorld()->CreateBody(&bodyDef);
    this->friction = friction;
    this->restitution = restitution;
    this->density = density;
    }

// -----------------------------------------------------------------------------

Box2dBody::~Box2dBody()
    {
    }

// -----------------------------------------------------------------------------

void Box2dBody::addCircleShape(float x, float y, float radius)
    {
    b2CircleDef shapeDef;
    shapeDef.localPosition.Set(x, y);
    shapeDef.radius = radius;
    shapeDef.friction = friction;
    shapeDef.restitution = restitution;
    shapeDef.density =density;
    //
    body->CreateShape(&shapeDef);
    body->SetMassFromShapes();
    }

// -----------------------------------------------------------------------------

void Box2dBody::addBoxShape(float x, float y, float w, float h)
    {
    b2PolygonDef shapeDef;
    shapeDef.SetAsBox(w, h, b2Vec2(x, y), 0);
    shapeDef.friction = friction;
    shapeDef.restitution = restitution;
    shapeDef.density = density;
    //
    body->CreateShape(&shapeDef);
    body->SetMassFromShapes();
    }

// -----------------------------------------------------------------------------

void Box2dBody::setPosition(float x, float y)
    {
    body->SetXForm(b2Vec2(x, y), body->GetAngle());
    }

// -----------------------------------------------------------------------------

void Box2dBody::applyForce(float forceX, float forceY, float pointX, float pointY)
    {
    if(!body->IsSleeping())
        body->ApplyForce(b2Vec2(forceX, forceY), b2Vec2(pointX, pointY));
    }

// -----------------------------------------------------------------------------

void Box2dBody::applyTorque(float torque)
    {
    if(!body->IsSleeping())
        body->ApplyTorque(torque);
    }

// -----------------------------------------------------------------------------

void Box2dBody::applyImpulse(float impulseX, float impulseY, float pointX, float pointY)
    {
    if(!body->IsSleeping())
        body->ApplyImpulse(b2Vec2(impulseX, impulseY), body->GetPosition());
    }

// -----------------------------------------------------------------------------

void Box2dBody::fillSharedObject()
    {
    shared_object.x = body->GetPosition().x;
    shared_object.y = body->GetPosition().y;
    shared_object.angle = -body->GetAngle() * 180/b2_pi;
    }

// -----------------------------------------------------------------------------
