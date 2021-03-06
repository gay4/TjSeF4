﻿#pragma once

/*单位的基类，提供接口，与UnitManager直接交互*/

#include "cocos2d.h"
#include "GridMap.h"
#include "UnitManager.h"
#include <map>
#include "ui/CocosGUI.h"


using std::map;
USING_NS_CC;
using namespace ui;

class UnitManager;
class Unit :public Sprite {
	
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
	friend class Bullet;
public:
	//CREATE_FUNC(Unit);

	//default ctr
	Unit();//

	static Unit* create(const std::string& filename);
	
	enum State {
		WONDERING = 0,//发呆中，可能会触发自动攻击
		MOVING,//移动
		NOSTATE,//建筑专用
		TRACING,//追击
	};

	enum Type {
		TANK = 0,//坦克
		SOILDER,//士兵
		FIGHTER,//飞机
		SOLDIERCAMP,//兵营
		FACTORY,//坦克和飞机工厂
		BASE,//基地
		MINE,//矿场
		ELECTRICITYFACTORY,//电厂
		//TOWER,//防御塔
	};
//protected:
	virtual void addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap);

	int _id = -1;//每个Unit的id都是不一样的
	
	GridMap* _gridMap = nullptr;

	Grid _finalDest;//最终目的地
	Grid _curDest;//当前帧要去的点

	GridMap::GridVector _gridPath;//本单位当前路径

	virtual void setProperties();//后期改为纯虚函数，具体兵需要重载该方法

	void setGridPath(const GridMap::GridVector& gPath);//call by manager

	void setDestination(const Grid& g);//call by manager

	//maybe send message to network through manager in this function
	void findPath();

	void moveTest();

	virtual GridMap::GridVector getPath(const Grid& dest);

protected:
	
	virtual void setOccupiedArea(GridMap::GridVector area)//建筑类重载该函数
	{/*empty*/ }
	virtual GridMap::GridVector getOccupiedArea()
	{
		return GridMap::GridVector();//空数组
	}

	virtual bool isBuilding()
	{
		return false;
	}

	void setId(int id)
	{
		_id = id;
	}

	void setManager(UnitManager* unitManager)
	{
		_unitManager = unitManager;
	}

	void setState(Unit::State state)
	{
		_state = state;
	}
	void setTraceId(int id)
	{
		//assert(id > 0);
		_traceId = id;
	}

	void autoAttack();

	//运动的实际逻辑
	void move();

	void shoot(Unit* attackee);

	//初始化血条
	void initHp();
	//更新血条
	void updateHp();

protected:
	UnitManager * _unitManager = nullptr;

	float _hpInterval;//血条更新量
	LoadingBar* _hp = nullptr;

	
	int _state = 4;//当前的状态

	float _moveSpeed;//移动速度
	int _attackCd;//距离上一次攻击的帧数
	int _attackCdMax;//每次攻击冷却需要的时间
	float _attackRange;//攻击范围
	int _timer = 0;//内部计数器

	int _attackEffect;//攻击伤害
	int _lifeValue;//当前生命值
	int _lifeValueMax;//最大生命值
	std::string _attackObject = "";//攻击时释放的东西
	int _traceId = -1;//追击目标Id

public:
	virtual void update(float delta);

private:
	//封装传递攻击消息的逻辑，供autoatk和trace使用
	void sendAttackMsg(int targetId);
	void trace();
	bool inAtkRange(Unit* unit);
	void attack(int id);//追击时的攻击
};

