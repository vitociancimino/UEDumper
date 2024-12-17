#pragma once
#include "stdafx.h"

#include "Engine/structs.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/ObjectsManager.h"
#include "Engine/Userdefined/Datatypes.h"

//any linked source code means that since then the code works and is unchanged
// more links means any changes in the versions


#if UE_VERSION >= UE_4_25
class FField;
class FProperty;
#endif
class UProperty;
class UClass;


// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBase.h#L222
// https://github.com/EpicGames/UnrealEngine/blob/5.3/Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBase.h#L246
/**
 * \brief The base class of all UE4 objects.
 */
class UObject
{
public:

#if defined(DFHO)

	uint64_t                                           objectptr = 0;                       // 0x0000   (0x0008)  
	unsigned char                                      UnknownData00_6[0x8];				// 0x0008   (0x0008)  MISSED
	class UClass* ClassPrivate = nullptr;													// 0x0010   (0x0008)  
	class UObject* OuterPrivate = nullptr;													// 0x0018   (0x0008)  
	EObjectFlags                                       ObjectFlags = RF_NoFlags;            // 0x0020   (0x0004)  
	FName                                              NamePrivate = FName();               // 0x0024   (0x0008)  
	int32_t                                            InternalIndex = 0;				    // 0x002C   (0x0004)

#else
	uint64_t			objectptr = 0;            // 0x0000 (0x0008)
	EObjectFlags		ObjectFlags = RF_NoFlags; // 0x0008 (0x0004)
	int32_t				InternalIndex = 0;	   // 0x000C (0x0004)
	class UClass* ClassPrivate = nullptr; // 0x0010 (0x0008)
	FName				NamePrivate = FName(); // 0x0018 (0x0008)
	class UObject* OuterPrivate = nullptr; // 0x0020 (0x0008)
#endif

	/** Object this object resides in. */

	static std::string typeName() { return "UObject"; }

	bool operator==(const UObject obj) const { return obj.objectptr == objectptr; }
	bool operator==(const UObject* obj) const { return obj->objectptr == objectptr; }
	bool operator!=(const UObject obj) const { return obj.objectptr != objectptr; }

	operator bool() const { return objectptr != 0; }

	UObject* getOuter() const;

	UClass* getClass() const;

	std::string getName(bool uProperty = false) const;

	static std::string getName(const FName& fName);


	std::string getFullName() const;

	std::string getCName();

	UObject* getOwnPointer() const;

	//real object
	UObject* getPackageObject() const;
	//fn pointrer
	UObject* getPackageObjectFnPtr() const;

	std::string getSecondPackageName() const;

	//header code otherwise linker errors
	/**
	 * \brief casts the object to a different object. WARNING: OBJECT NEEDS TO EXIST OR NULLPTR
	 * \tparam T type
	 * \return casted object
	 */
	template <typename T>
	T* castTo()
	{
		//T obj = EngineCore::getUObject<T>(reinterpret_cast<uint64_t>(getOwnPointer()));
		return ObjectsManager::getUObject<T>(objectptr);
	}

	bool IsA(const UClass* cmp) const;

	template <typename T>
	bool IsA()
	{
		auto staticClass = T::staticClass();
		if (!staticClass)
			return false;

		return IsA(staticClass);
	}


	static UClass* staticClass();
};

/**
 * \brief AActor class just used fot the static name
 */
class AActor : UObject
{
public:
	using UObject::UObject;

	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L56
// https://github.com/EpicGames/UnrealEngine/blob/5.3/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L155
//
// Base class of reflection data objects.
//
class UField : public UObject {
public:
	using UObject::UObject;

	/** Next Field in the linked list */
	UField* Next;  // 0x0030 (0x0008)

	UField* getNext() const;

	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L1495
// https://github.com/EpicGames/UnrealEngine/blob/5.3/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L1999
//
// Reflection data for an enumeration.
//
class UEnum : public UField
{
public:
	using UField::UField;

	enum class ECppForm
	{
		Regular,
		Namespaced,
		EnumClass
	};

	/** This will be the true type of the enum as a string, e.g. "ENamespacedEnum::InnerType" or "ERegularEnum" or "EEnumClass" */
	FString							CppType;

	/** List of pairs of all enum names and values. */
	TArray<TPair<FName, int64_t>>	Names;

	/** How the enum was originally defined. */
	ECppForm						CppForm;

	//additional data is not needed, as UEnum is not inherited by any class


	std::vector<TPair<FName, int64_t>> getNames() const;

	//static std::string typeName() { return "UEnum"; }
	static UClass* staticClass();
};

#if UE_VERSION >= UE_4_22
class FStructBaseChain
{
public:
	FStructBaseChain** StructBaseChainArray;
	int32_t NumStructBasesInChainMinusOne;
};

#endif

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L218
// https://github.com/EpicGames/UnrealEngine/blob/4.22/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L248
// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L283
// https://github.com/EpicGames/UnrealEngine/blob/5.3/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L383

/**
 * \brief Base class for all UObject types that contain fields.
 */
class UStruct : public UField
{
public:
	using UField::UField;

	//start offset 0x38 Size 0xDC

	unsigned char UnknownData00_3[0x10];	 // 0x0038 (0x0010)
	int32_t		PropertiesSize;				 // 0x0048 (0x0004)
	unsigned char UnknownData00_4[0x4];		 // 0x004C (0x0004)
	UStruct* SuperStruct;					 // 0x0050 (0x0008)
	int32_t		MinAlignment;				 // 0x0058 (0x0004)
	unsigned char UnknownData00_5[0x4];      // 0x005C (0x0004)
	UField* Children;						 // 0x0080 (0x0008)
	unsigned char UnknownData00_6[0x20];     // 0x0060 (0x0020)
	unsigned char UnknownData02_7[0x8C];     // 0x0088 (0x008C) 


	//TArray<uint8_t> Script;
	//UProperty* PropertyLink;
	//UProperty* RefLink;
	//UProperty* DestructorLink;
	//UProperty* PostConstructLink;
	//TArray<UObject*> ScriptObjectReferences;


	template <typename T>
	T* getSuper();

	UStruct* getSuper() const;

	std::vector<UObject*> getAllSupers() const;

	template <typename T>
	T* getChildren();

	UField* getChildren() const;

	static UClass* staticClass();
};


class UScriptStruct : public UStruct
{
public:
	using UStruct::UStruct;

	//static std::string typeName() { return "UScriptStruct"; }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h#L1297
/**
 * \brief Reflection data for a replicated or Kismet callable function.
 */
class UFunction : public UStruct
{
public:
	using UStruct::UStruct;

	/** EFunctionFlags set defined for this function */
	EFunctionFlags	FunctionFlags;

	/** Number of parameters total */
	uint8_t NumParms;

	/** Total size of parameters in memory */
	uint16_t ParmsSize;

	/** Memory offset of return value property */
	uint16_t ReturnValueOffset;

	/** Id of this RPC function call (must be FUNC_Net & (FUNC_NetService|FUNC_NetResponse)) */
	uint16_t RPCId;

	/** Id of the corresponding response call (must be FUNC_Net & FUNC_NetService) */
	uint16_t RPCResponseId;


	/** pointer to first local struct property in this UFunction that contains defaults */
	UProperty* FirstPropertyToInit;

#if UE_BLUEPRINT_EVENTGRAPH_FASTCALLS
	/** The event graph this function calls in to (persistent) */
	UFunction* EventGraphFunction;

	/** The state offset inside of the event graph (persistent) */
	int32_t EventGraphCallOffset;
#endif

#if UE_VERSION >= UE_5_00
#if WITH_LIVE_CODING
	/** Pointer to the cached singleton pointer to this instance */
	UFunction** SingletonPtr;
#endif
#endif

	/** C++ function this is bound to */
	uintptr_t		Func;

	/** String function flags */
	std::string getFunctionFlagsString() const;

	static UClass* staticClass();
};

//class not really used
class UClass : public UStruct
{
public:
	using UStruct::UStruct;


	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L71
/**
 * \brief An UnrealScript variable.
 */
class UProperty : public UField
{
public:
	using UField::UField;

	// Persistent variables.
	char pad[4];				// 0x0038 (0x0004)

	int32_t		ArrayDim;		// 0x003C (0x0004)
	int32_t		ElementSize;	// 0x0040 (0x0004)
	char pad2[4];				// 0x0044 (0x0004)
	uint64_t	PropertyFlags;	// 0x0048 (0x0008)
	uint16_t	RepIndex; 	    // 0x0050 (0x0002)

	uint8_t		BlueprintReplicationCondition;  // 0x0052 (0x0001)
	int32_t		Offset; // 0x0054 (0x0004)

	FName		RepNotifyFunc; 

	/** In memory only: Linked list of properties from most-derived to base **/
	UProperty* PropertyLinkNext;

	/** In memory only: Linked list of object reference properties from most-derived to base **/
	UProperty* NextRef;

	/** In memory only: Linked list of properties requiring destruction. Note this does not include things that will be destroyed byt he native destructor **/
	UProperty* DestructorLinkNext;

	/** In memory only: Linked list of properties requiring post constructor initialization.**/
	UProperty* PostConstructLinkNext;

	static std::string typeName() { return "UProperty"; }
	static UClass* staticClass();

	int32_t getOffset() const;

	fieldType getType();

};


class UNumericProperty : public UProperty
{
public:
	using UProperty::UProperty;

	//static std::string typeName() { return "NUMERIC"; }
	static UClass* staticClass();
};

class UByteProperty : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	uintptr_t	Enum;

	UEnum* getEnum() const;

	std::string typeName() const { return Enum ? "TEnumAsByte" : TYPE_UCHAR; }

	//only if Enum exists!
	std::vector<fieldType> getSubTypes() const
	{
		if (!Enum)
			return {};

		const auto enu = getEnum();
		if (!enu)
			return {};
		return std::vector<fieldType>{ {true, PropertyType::EnumProperty, enu->getName()}};
	};
	static UClass* staticClass();
};

class UUInt16Property : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_UI16; }
	static UClass* staticClass();

};

class UUInt32Property : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_UI32; }
	static UClass* staticClass();

};

class UUInt64Property : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_UI64; }
	static UClass* staticClass();

};

class UInt8Property : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_I8; }
	static UClass* staticClass();

};

class UInt16Property : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_I16; }
	static UClass* staticClass();

};

class UIntProperty : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_I32; }
	static UClass* staticClass();
};

class UInt64Property : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_I64; }
	static UClass* staticClass();
};

class UFloatProperty : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_FLOAT; }
	static UClass* staticClass();
};

class UDoubleProperty : public UNumericProperty
{
public:
	using UNumericProperty::UNumericProperty;

	static std::string typeName() { return TYPE_DOUBLE; }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L1705
class UBoolProperty : public UProperty
{
public:
	using UProperty::UProperty;

	/** Size of the bitfield/bool property. Equal to ElementSize but used to check if the property has been properly initialized (0-8, where 0 means uninitialized). */
	uint8_t		FieldSize;

	/** Offset from the member variable to the byte of the property (0-7). */
	uint8_t		ByteOffset;

	/** Mask of the byte with the property value. */
	uint8_t		ByteMask;

	/** Mask of the field with the property value. Either equal to ByteMask or 255 in case of 'bool' type. */
	uint8_t		FieldMask;


	static int getBitPosition(uint8_t byteMask);

	struct BitInfo
	{
		int bitPos;
		int additionalBitPos;
	};

	BitInfo getMissingBitsCount(const UBoolProperty& lastProp) const;

	std::string typeName() const { return FieldMask == 255 || FieldMask == ByteMask ? TYPE_BOOLEAN : TYPE_UCHAR; }

	bool isBitField() const { return FieldMask != 0xFF; }

	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L1845
class UObjectPropertyBase : public UProperty
{
public:
	using UProperty::UProperty;

	UClass* PropertyClass;

	UClass* getPropertyClass() const;

	std::string typeName() const { return getPropertyClass()->getCName(); }
	static std::string weakTypeName() { return "TWeakObjectPtr"; }
	static std::string softTypeName() { return "TSoftObjectPtr"; }
	static std::string lazyTypeName() { return "TLazyObjectPtr"; }
	//only use on wak or lazy types!
	std::vector<fieldType> getSubTypes() const { return std::vector<fieldType>{ {true, PropertyType::ObjectProperty, getPropertyClass()->getCName()}}; }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2031
class UObjectProperty : public UObjectPropertyBase
{
public:
	using UObjectPropertyBase::UObjectPropertyBase;

	//static std::string typeName() { return "UObjectProperty"; }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2194
class UClassProperty : public UObjectProperty
{
public:
	using UObjectProperty::UObjectProperty;

	UClass* MetaClass;

	UClass* getMetaClass() const;

	std::string typeName() const { return getPropertyClass()->getCName(); }

	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2319
class UInterfaceProperty : public UProperty
{
public:
	using UProperty::UProperty;

	UProperty* InterfaceClass;

	UProperty* getInterfaceClass() const;

	static std::string typeName() { return "TScriptInterface"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{ getInterfaceClass()->getType() }; }

	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2071
class UWeakObjectProperty : public UObjectPropertyBase
{
public:
	using UObjectPropertyBase::UObjectPropertyBase;

	//std::string typeName();
	static UClass* staticClass();
};

class USoftObjectProperty : public UObjectPropertyBase
{
public:
	using UObjectPropertyBase::UObjectPropertyBase;

	//std::string typeName();
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2104
class ULazyObjectProperty : public UObjectPropertyBase
{
public:
	using UObjectPropertyBase::UObjectPropertyBase;

	//static std::string typeName() { return "ULazyObjectProperty"; }
	static UClass* staticClass();
};

class UAssetObjectProperty : public UObjectPropertyBase
{
public:
	using UObjectPropertyBase::UObjectPropertyBase;

	//static std::string typeName() { return "UAssetObjectProperty"; }
	static UClass* staticClass();
};

class UAssetClassProperty : public UAssetObjectProperty
{
public:
	using UAssetObjectProperty::UAssetObjectProperty;

	uintptr_t	MetaClass;

	//static std::string typeName() { return "UAssetClassProperty"; }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2392
class UNameProperty : public UProperty
{
public:
	using UProperty::UProperty;

	static std::string typeName() { return "FName"; }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L4014
class UStructProperty : public UProperty
{
public:
	using UProperty::UProperty;

	UScriptStruct* Struct;

	UScriptStruct* getStruct() const;

	std::string typeName() const { return getStruct()->getCName(); }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2429
class UStrProperty : public UProperty
{
public:
	using UProperty::UProperty;

	static std::string typeName() { return "FString"; }
	static UClass* staticClass();
};

class UTextProperty : public UProperty
{
public:
	using UProperty::UProperty;

	static std::string typeName() { return "FText"; }
	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2469
class UArrayProperty : public UProperty
{
public:
	using UProperty::UProperty;

	UProperty* Inner;

	UProperty* getInner() const;

	static std::string typeName() { return "TArray"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{ getInner()->getType() }; }

	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2528
class UMapProperty : public UProperty
{
public:
	using UProperty::UProperty;

	UProperty* KeyProp;
	UProperty* ValueProp;

	UProperty* getKeyProp() const;

	UProperty* getValueProp() const;

	static std::string typeName() { return "TMap"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{ getKeyProp()->getType(), getValueProp()->getType() }; }
	static UClass* staticClass();
};

class USetProperty : public UProperty
{
public:
	UProperty* ElementProp;

	UProperty* getElementProp() const;

	static std::string typeName() { return "TSet"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{ getElementProp()->getType() }; }
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L4087
// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L5926
class UDelegateProperty : public UProperty
{
public:
	using UProperty::UProperty;

	UFunction* SignatureFunction;

#if UE_VERSION < UE_4_25
	static std::string typeName() { return "UDelegateProperty"; }
#else
	static std::string typeName() { return "FDelegateProperty"; }
#endif


	static UClass* staticClass();
};

// https://github.com/EpicGames/UnrealEngine/blob/4.19/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L4140
// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L6000
class UMulticastDelegateProperty : public UProperty
{
public:
	using UProperty::UProperty;

	UFunction* SignatureFunction;

#if UE_VERSION < UE_4_25
	static std::string typeName() { return "UMulticastDelegate"; }
#else
	static std::string typeName() { return "FMulticastDelegate"; }
#endif
	static UClass* staticClass();
};

class UEnumProperty : public UProperty
{
public:
	using UProperty::UProperty;

	uintptr_t	UnderlyingProp;
	UEnum* Enum;

	UEnum* getEnum() const;

	std::string typeName() const
	{
		if (const auto enu = getEnum())
			return enu->getName();
		return "";
	}

	static UClass* staticClass();
};


/*
 *
 *
 * >= 4.25 RELATED
 *
 *
 */

#if UE_VERSION >= UE_4_25

class FFieldVariant;

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/Field.h#L34
/**
 * \brief Object representing a type of an FField struct.
  * Mimics a subset of UObject reflection functions.
 */
class FFieldClass
{
public:
	/** Name of this field class */
	FName Name;

	/** Unique Id of this field class (for casting) */
	uint64_t Id;

	/** Cast flags used for casting to other classes */
	uint64_t CastFlags;
	/** Class flags */
	EClassFlags ClassFlags;
	/** Super of this class */
	FFieldClass* SuperClass;
	/** Default instance of this class */
	FField* DefaultObject;
	/** Pointer to a function that can construct an instance of this class */
	FField* (*ConstructFn)(const FFieldVariant&, const FName&, EObjectFlags);
	/** Counter for generating runtime unique names */
	uint32_t UnqiueNameIndexCounter;

	std::string getName() const;
};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/Field.h#L202
/**
 * \brief Special container that can hold either UObject or FField.
 * Exposes common interface of FFields and UObjects for easier transition from UProperties to FProperties.
 * DO NOT ABUSE. IDEALLY THIS SHOULD ONLY BE FFIELD INTERNAL STRUCTURE FOR HOLDING A POINTER TO THE OWNER OF AN FFIELD.
 */
class FFieldVariant
{
public:
	union FFieldObjectUnion
	{
		FField* Field;
		UObject* Object;
	} Container;

#if UE_VERSION < UE_5_03
	bool bIsUObject;
#endif
};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/Field.h#L351
// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/Field.h#L261
// https://github.com/EpicGames/UnrealEngine/blob/5.3/Engine/Source/Runtime/CoreUObject/Public/UObject/Field.h#L264
/**
 * \brief Base class of reflection data objects.
 */
class FField
{
public:
	//again replace vtable ptr with this
	uint64_t objectptr = 0;
	/** Pointer to the class object representing the type of this FField */
	FFieldClass* ClassPrivate;

	/** Owner of this field */
	FFieldVariant Owner;

	/** Next Field in the linked list */
	FField* Next;

	/** Name of this field */
	FName NamePrivate;

	/** Object flags */
	EObjectFlags FlagsPrivate;


	operator bool() const { return ClassPrivate != nullptr; }

	FProperty* getNext() const;

	std::string getName() const;

	template <typename T>
	T* castTo()
	{
		//T obj = EngineCore::getUObject<T>(reinterpret_cast<uint64_t>(getOwnPointer()));
		return ObjectsManager::getFField<T>(objectptr);
	}
};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L91
class FProperty : public FField
{
public:
	using FField::FField;

	// Persistent variables.
	int32_t			ArrayDim;
	int32_t			ElementSize;
	EPropertyFlags	PropertyFlags;
	uint16_t		RepIndex;

	uint8_t			BlueprintReplicationCondition;
	int32_t			Offset;

	FName			RepNotifyFunc;

	/** In memory only: Linked list of properties from most-derived to base **/
	FProperty* PropertyLinkNext;

	/** In memory only: Linked list of object reference properties from most-derived to base **/
	FProperty* NextRef;

	/** In memory only: Linked list of properties requiring destruction. Note this does not include things that will be destroyed byt he native destructor **/
	FProperty* DestructorLinkNext;

	/** In memory only: Linked list of properties requiring post constructor initialization.**/
	FProperty* PostConstructLinkNext;

	//static std::string typeName() { return "UProperty"; }
	//FIXMEEEE
	static UClass staticClass();

	int32_t getOffset() const;

	// this generates the field type for the given type. however, this will not add the Objectinfo as this has to be done manually at the very end of generation!
	fieldType getType();

};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L1997
class FObjectPropertyBase : public FProperty
{
public:
	UClass* PropertyClass;


	UClass* getPropertyClass() const;

	//std::string typeName() const { return "struct " + getPropertyClass().getCName() + "*"; }
	std::string typeName() const { return getPropertyClass()->getCName(); }
	static std::string weakTypeName() { return "TWeakObjectPtr"; }
	static std::string lazyTypeName() { return "TLazyObjectPtr"; }
	//only use on weak or lazy types!
	std::vector<fieldType> getSubTypes() const { return std::vector<fieldType>{ {true, PropertyType::ObjectProperty, getPropertyClass()->getCName()}}; }
};

// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L3405
class FSoftClassProperty : public FProperty
{
public:
	UClass* MetaClass;

	UClass* getMetaClass() const;

	static std::string typeName() { return "TSoftObjectPtr"; }

	std::vector<fieldType> getSubTypes() const { return std::vector<fieldType>{ {true, PropertyType::ObjectProperty, getMetaClass()->getCName()}}; }

};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L2734
// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L3702
// https://github.com/EpicGames/UnrealEngine/blob/5.3/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L3628
class FArrayProperty : public FProperty
{
public:
#if UE_VERSION < UE_5_03
	FProperty* Inner;
	EArrayPropertyFlags ArrayFlags;
#else
	EArrayPropertyFlags ArrayFlags;
	FProperty* Inner;
#endif

	FProperty* getInner() const;

	static std::string typeName() { return "TArray"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{ getInner()->getType() }; }
};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L4711
class FStructProperty : public FProperty
{
public:
	UStruct* Struct;

	UStruct* getStruct() const;

	std::string typeName() const { return getStruct()->getCName(); }

	//std::string typeName() const { return "struct " + getStruct().getCName(); }
};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L1864
class FBoolProperty : public FProperty
{
public:
	/** Size of the bitfield/bool property. Equal to ElementSize but used to check if the property has been properly initialized (0-8, where 0 means uninitialized). */
	uint8_t FieldSize;
	/** Offset from the member variable to the byte of the property (0-7). */
	uint8_t ByteOffset;
	/** Mask of the byte with the property value. */
	uint8_t ByteMask;
	/** Mask of the field with the property value. Either equal to ByteMask or 255 in case of 'bool' type. */
	uint8_t FieldMask;


	static int getBitPosition(uint8_t byteMask);

	struct BitInfo
	{
		int bitPos;
		int additionalBitPos;
	};

	BitInfo getMissingBitsCount(const FBoolProperty& lastProp) const;

	std::string typeName() const { return FieldMask == 255 || FieldMask == ByteMask ? TYPE_BOOLEAN : TYPE_UCHAR; }

	bool isBitField() const { return FieldMask != 0xFF; }
};


class FEnumProperty : public FProperty
{
public:
	FProperty* UnderlyingProp;
	UEnum* Enum;

	UEnum* getEnum() const;

	std::string typeName() const { return getEnum()->getName(); }

};

// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L3486
class FInterfaceProperty : public FProperty
{
public:
	UClass* InterfaceClass;

	UProperty* getInterfaceClass() const;

	//dont forget <>
	static std::string typeName() { return "TScriptInterface"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{ getInterfaceClass()->getType() }; }

	//std::string typeName() const { return "struct TScriptInterface<" + getInterfaceClass().getType().cName + ">"; }
};

// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L3846
class FMapProperty : public FProperty
{
public:
	FProperty* KeyProp;
	FProperty* ValueProp;

	//FScriptMapLayout MapLayout;
	//EMapPropertyFlags MapFlags;

	FProperty* getKeyProp() const;

	FProperty* getValueProp() const;

	static std::string typeName() { return "TMap"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{getKeyProp()->getType(), getValueProp()->getType()}; }

	//std::string typeName() const { return "struct TMap<" + getKeyProp().getType().cName + ", " + getValueProp().getType().cName + ">"; }
};

// https://github.com/EpicGames/UnrealEngine/blob/5.2/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L4004
class FSetProperty : public FProperty
{
public:
	FProperty* ElementProp;

	FProperty* getElementProp() const;

	static std::string typeName() { return "TSet"; }

	std::vector<fieldType> getSubTypes() const { return std::vector{ getElementProp()->getType() }; }
	//std::string typeName() const { return "struct TSet<" + getElementProp().getType().cName + ">"; }
};

class FFieldPathProperty : public FProperty
{
public:
	FFieldClass* PropertyClass;

	FFieldClass* getPropertyClass() const;
	static std::string typeName() { return "TFieldPath"; }
	std::vector<fieldType> getSubTypes() const { return std::vector<fieldType>{ {true, PropertyType::StructProperty, "F" + EngineCore::FNameToString(NamePrivate)}}; }
};

// https://github.com/EpicGames/UnrealEngine/blob/4.25/Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h#L1517
class FByteProperty : public FProperty
{
public:
	UEnum* Enum;

	UEnum* getEnum() const;

	std::string typeName() const { return Enum ? "TEnumAsByte" : TYPE_CHAR; }

	//only if Enum exists! Crash here? Then your UE version is probably wrong. If it still crashes, most likely offset error.
	std::vector<fieldType> getSubTypes() const { if (!Enum) { DebugBreak(); } return std::vector<fieldType>{ {true, PropertyType::EnumProperty, getEnum()->getName()}}; };
};

#endif