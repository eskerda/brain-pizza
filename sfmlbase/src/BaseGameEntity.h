#pragma once
//------------------------------------------------------------------------
//
//  Name: BaseGameEntity.h
//
//  Desc: Base class to define a common interface for all game
//        entities
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------


#include "constants.h"

#include <vector>
#include <string>

#include "math/Vector2D.h"
#include "math/geometry.h"
#include "math/utils.h"
#include "InputManager.h"

#include <SFML/Graphics.hpp>

struct Telegram;

class BaseGameEntity
{

protected:

  Vector2D m_vPos;
  float    m_dBoundingRadius;

  
  BaseGameEntity(Vector2D pos = Vector2D(0,0), float r = 0.0f)
      : m_vPos(pos)
      , m_dBoundingRadius(r)
  {
  
  }

public:

  virtual ~BaseGameEntity(){}

  virtual void Update(float time_elapsed) = 0;

  virtual void Draw(sf::RenderTarget& rt) = 0;

  virtual bool HandleMessage(const Telegram& msg){return false;}
  
  Vector2D     Pos()const{return m_vPos;}
  void         SetPos(Vector2D new_pos){m_vPos = new_pos;}
  void         SetPos(float x, float y){m_vPos.x = x; m_vPos.y = y;}

  float        BRadius()const{return m_dBoundingRadius;}
  void         SetBRadius(float r){m_dBoundingRadius = r;}

};



