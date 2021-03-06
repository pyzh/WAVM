#include <stdint.h>
#include <cmath>
#include <string>
#include <vector>

#include "IR/IR.h"
#include "IR/Types.h"
#include "Inline/Assert.h"
#include "Inline/BasicTypes.h"
#include "Inline/Floats.h"
#include "Logging/Logging.h"
#include "Runtime/Intrinsics.h"
#include "Runtime/Runtime.h"
#include "Runtime/RuntimeData.h"
#include "RuntimePrivate.h"

using namespace Runtime;

namespace Runtime
{
	DEFINE_INTRINSIC_MODULE(wavmIntrinsics)
}

template<typename Float> Float quietNaN(Float value)
{
	Floats::FloatComponents<Float> components;
	components.value = value;
	components.bits.significand |= typename Floats::FloatComponents<Float>::Bits(1)
								   << (Floats::FloatComponents<Float>::numSignificandBits - 1);
	return components.value;
}

template<typename Float> Float floatMin(Float left, Float right)
{
	// If either operand is a NaN, convert it to a quiet NaN and return it.
	if(left != left) { return quietNaN(left); }
	else if(right != right)
	{
		return quietNaN(right);
	}
	// If either operand is less than the other, return it.
	else if(left < right)
	{
		return left;
	}
	else if(right < left)
	{
		return right;
	}
	else
	{
		// Finally, if the operands are apparently equal, compare their integer values to
		// distinguish -0.0 from +0.0
		Floats::FloatComponents<Float> leftComponents;
		leftComponents.value = left;
		Floats::FloatComponents<Float> rightComponents;
		rightComponents.value = right;
		return leftComponents.bitcastInt < rightComponents.bitcastInt ? right : left;
	}
}

template<typename Float> Float floatMax(Float left, Float right)
{
	// If either operand is a NaN, convert it to a quiet NaN and return it.
	if(left != left) { return quietNaN(left); }
	else if(right != right)
	{
		return quietNaN(right);
	}
	// If either operand is less than the other, return it.
	else if(left > right)
	{
		return left;
	}
	else if(right > left)
	{
		return right;
	}
	else
	{
		// Finally, if the operands are apparently equal, compare their integer values to
		// distinguish -0.0 from +0.0
		Floats::FloatComponents<Float> leftComponents;
		leftComponents.value = left;
		Floats::FloatComponents<Float> rightComponents;
		rightComponents.value = right;
		return leftComponents.bitcastInt > rightComponents.bitcastInt ? right : left;
	}
}

template<typename Float> Float floatCeil(Float value)
{
	if(value != value) { return quietNaN(value); }
	else
	{
		return ceil(value);
	}
}

template<typename Float> Float floatFloor(Float value)
{
	if(value != value) { return quietNaN(value); }
	else
	{
		return floor(value);
	}
}

template<typename Float> Float floatTrunc(Float value)
{
	if(value != value) { return quietNaN(value); }
	else
	{
		return trunc(value);
	}
}

template<typename Float> Float floatNearest(Float value)
{
	if(value != value) { return quietNaN(value); }
	else
	{
		return nearbyint(value);
	}
}

DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f32.min", F32, f32Min, F32 left, F32 right)
{
	return floatMin(left, right);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f64.min", F64, f64Min, F64 left, F64 right)
{
	return floatMin(left, right);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f32.max", F32, f32Max, F32 left, F32 right)
{
	return floatMax(left, right);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f64.max", F64, f64Max, F64 left, F64 right)
{
	return floatMax(left, right);
}

DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f32.ceil", F32, f32Ceil, F32 value)
{
	return floatCeil(value);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f64.ceil", F64, f64Ceil, F64 value)
{
	return floatCeil(value);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f32.floor", F32, f32Floor, F32 value)
{
	return floatFloor(value);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f64.floor", F64, f64Floor, F64 value)
{
	return floatFloor(value);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f32.trunc", F32, f32Trunc, F32 value)
{
	return floatTrunc(value);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f64.trunc", F64, f64Trunc, F64 value)
{
	return floatTrunc(value);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f32.nearest", F32, f32Nearest, F32 value)
{
	return floatNearest(value);
}
DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "f64.nearest", F64, f64Nearest, F64 value)
{
	return floatNearest(value);
}

DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics,
						  "divideByZeroOrIntegerOverflowTrap",
						  void,
						  divideByZeroOrIntegerOverflowTrap)
{
	throwException(Exception::integerDivideByZeroOrOverflowType);
}

DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "unreachableTrap", void, unreachableTrap)
{
	throwException(Exception::reachedUnreachableType);
}

DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "accessViolationTrap", void, accessViolationTrap)
{
	throwException(Exception::memoryAddressOutOfBoundsType);
}

DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics,
						  "invalidFloatOperationTrap",
						  void,
						  invalidFloatOperationTrap)
{
	throwException(Exception::invalidFloatOperationType);
}

DEFINE_INTRINSIC_FUNCTION(wavmIntrinsics, "debugBreak", void, debugBreak)
{
	Log::printf(Log::debug, "================== wavmIntrinsics.debugBreak\n");
}

Runtime::ModuleInstance* Runtime::instantiateWAVMIntrinsics(Compartment* compartment)
{
	dummyReferenceAtomics();
	return Intrinsics::instantiateModule(
		compartment, INTRINSIC_MODULE_REF(wavmIntrinsics), "WAVMIntrinsics");
}
