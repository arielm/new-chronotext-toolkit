/*
 * FROM SAFETYDANK'S FORK OF CINDER:
 * https://github.com/safetydank/Cinder/blob/fdad2a623be2caf2e371884cba1a2ef92616220b/include/cinder/BinPack.h
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Area.h"

namespace cinder {

class BinPack;
typedef std::shared_ptr<BinPack> BinPackRef;

class BinPack
{
public:
	enum PackType {
		DEFAULT     = 0,
		SKYLINE     = 1,
		BINARY_TREE = 2,
	};

	static BinPackRef create( int width, int height, PackType type = DEFAULT );

	virtual Area allocateArea( int width, int height ) = 0;
	virtual ~BinPack();
};

}
