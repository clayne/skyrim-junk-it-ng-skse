#pragma once

#define PI 3.1415926535897932f
#define TWOTHIRDS_PI 2.0943951023931955f
#define TWO_PI 6.2831853071795865f
#define PI2 1.5707963267948966f
#define PI3 1.0471975511965977f
#define PI4 0.7853981633974483f
#define PI8 0.3926990816987242f

using namespace RE;

[[nodiscard]] inline RE::hkVector4 NiPointToHkVector(const RE::NiPoint3& pt) { return { pt.x, pt.y, pt.z, 0 }; };


namespace PointerUtil //yoinked po3's code
{
template <class T, class U>
inline auto adjust_pointer(U* a_ptr, std::ptrdiff_t a_adjust) noexcept
{
	auto addr = a_ptr ? reinterpret_cast<std::uintptr_t>(a_ptr) + a_adjust : 0;
	if constexpr (std::is_const_v<U> && std::is_volatile_v<U>) {
		return reinterpret_cast<std::add_cv_t<T>*>(addr);
	} else if constexpr (std::is_const_v<U>) {
		return reinterpret_cast<std::add_const_t<T>*>(addr);
	} else if constexpr (std::is_volatile_v<U>) {
		return reinterpret_cast<std::add_volatile_t<T>*>(addr);
	} else {
		return reinterpret_cast<T*>(addr);
	}
}
}
namespace SystemUtil
{
    struct File 
    {
        
	static std::vector<std::string> GetConfigs(std::string_view a_folder, std::string_view a_suffix, std::string_view a_extension = ".ini"sv)
	{
		std::vector<std::string> configs{};

		for (const auto iterator = std::filesystem::directory_iterator(a_folder); const auto& entry : iterator) {
			if (entry.exists()) {
				if (const auto& path = entry.path(); !path.empty() && path.extension() == a_extension) {
					if (const auto& fileName = entry.path().string(); fileName.rfind(a_suffix) != std::string::npos) {
						configs.push_back(fileName);
					}
				}
			}
		}

		std::ranges::sort(configs);

		return configs;
	}
    };
}

namespace KeyUtil 
{

    enum class MACRO_LIMITS {
        kMaxMacros = 282
    };

    enum class KBM_OFFSETS {
		// first 256 for keyboard, then 8 mouse buttons, then mouse wheel up, wheel down, then 16 gamepad buttons
		kMacro_KeyboardOffset = 0,		// not actually used, just for self-documentation
		kMacro_NumKeyboardKeys = 256,

		kMacro_MouseButtonOffset = kMacro_NumKeyboardKeys,	// 256
		kMacro_NumMouseButtons = 8,

		kMacro_MouseWheelOffset = kMacro_MouseButtonOffset + kMacro_NumMouseButtons,	// 264
		kMacro_MouseWheelDirections = 2,

		kMacro_GamepadOffset = kMacro_MouseWheelOffset + kMacro_MouseWheelDirections,	// 266
		kMacro_NumGamepadButtons = 16,

			// 282
	};

	enum class GAMEPAD_OFFSETS {
		kGamepadButtonOffset_DPAD_UP = static_cast<int>(KBM_OFFSETS::kMacro_GamepadOffset),	// 266
		kGamepadButtonOffset_DPAD_DOWN,
		kGamepadButtonOffset_DPAD_LEFT,
		kGamepadButtonOffset_DPAD_RIGHT,
		kGamepadButtonOffset_START,
		kGamepadButtonOffset_BACK,
		kGamepadButtonOffset_LEFT_THUMB,
		kGamepadButtonOffset_RIGHT_THUMB,
		kGamepadButtonOffset_LEFT_SHOULDER,
		kGamepadButtonOffset_RIGHT_SHOULDER,
		kGamepadButtonOffset_A,
		kGamepadButtonOffset_B,
		kGamepadButtonOffset_X,
		kGamepadButtonOffset_Y,
		kGamepadButtonOffset_LT,
		kGamepadButtonOffset_RT	// 281
	};


    struct Interpreter
    {
        public: 
        static uint32_t GamepadMaskToKeycode(uint32_t keyMask) {
	switch (keyMask) {
		case 0x001:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_UP);
		case 0x002:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_DOWN);
		case 0x004:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_LEFT);
		case 0x008:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_RIGHT);
		case 0x0010:			return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_START);
		case 0x0020:			return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_BACK);
		case 0x0040:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_LEFT_THUMB);
		case 0x0080:	return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_RIGHT_THUMB);
		case 0x0100:	return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_LEFT_SHOULDER);
		case 0x0200: return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_RIGHT_SHOULDER);
		case 0x1000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_A);
		case 0x2000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_B);
		case 0x4000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_X);
		case 0x8000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_Y);
		case 0x9:							return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_LT);
		case 0xA:							return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_RT);
		default:							return 282; // Invalid
	}
}

    };

}

namespace Util
{
    struct String
    {
		static std::vector<std::string> Split(const std::string& a_str, std::string_view a_delimiter)
		{
			auto range = a_str | std::ranges::views::split(a_delimiter) | std::ranges::views::transform([](auto&& r) { return std::string_view(r); });
			return { range.begin(), range.end() };
		}

        static bool iContains(std::string_view a_str1, std::string_view a_str2)
		{
			if (a_str2.length() > a_str1.length()) {
				return false;
			}

			const auto subrange = std::ranges::search(a_str1, a_str2, [](unsigned char ch1, unsigned char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			});

			return !subrange.empty();
		}

		static bool iEquals(std::string_view a_str1, std::string_view a_str2)
		{
			return std::ranges::equal(a_str1, a_str2, [](unsigned char ch1, unsigned char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			});
		}

		// https://stackoverflow.com/a/35452044
		static std::string Join(const std::vector<std::string>& a_vec, std::string_view a_delimiter)
		{
			return std::accumulate(a_vec.begin(), a_vec.end(), std::string{},
				[a_delimiter](const auto& str1, const auto& str2) {
					return str1.empty() ? str2 : str1 + a_delimiter.data() + str2;
				});
		}

        static std::vector<float> ToFloatVector(const std::vector<std::string> stringVector)
        {
            std::vector<float> floatNumbers; 
            for(auto str : stringVector)
            {
                float num = static_cast<float>(atof(str.c_str()));
                floatNumbers.push_back(num);
            }
            return floatNumbers;
        }
        static std::string ToLower(std::string_view a_str)
		{
			std::string result(a_str);
			std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::tolower(ch)); });
			return result;
		}

		static std::string ToUpper(std::string_view a_str)
		{
			std::string result(a_str);
			std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::toupper(ch)); });
			return result;
		}


    };

    struct Numbers
    {
        static float GenerateRandomFloat(float afMin, float afMax)
        {
            std::random_device rd;

            std::mt19937 engine{rd()};

            std::uniform_real_distribution<float> dist(afMin, afMax);

            return dist(engine);
        }

        static uint32_t GenerateRandomInt(uint32_t afMin, uint32_t afMax)
        {
            std::random_device rd;

            std::mt19937 engine{rd()};

            std::uniform_int_distribution<uint32_t> dist(afMin, afMax);

            return dist(engine);
        }
    };

}

namespace MathUtil
{
    struct Angle 
    {
        [[nodiscard]] constexpr static float DegreeToRadian(float a_angle)
        {
            return a_angle * 0.017453292f;
        }

        [[nodiscard]] constexpr static float RadianToDegree(float a_radian)
        {
            return a_radian * 57.295779513f;
        }

        static NiPoint3 ToRadianVector(float x, float y, float z)
        {
            RE::NiPoint3 rotationVector{ 0.f, 0.f, 0.f };

            rotationVector.x = DegreeToRadian(x); 
            rotationVector.y = DegreeToRadian(y); 
            rotationVector.z = DegreeToRadian(z); 
            return rotationVector; 
        }

        static float NormalAbsoluteAngle(float a_angle)
        {
            while (a_angle < 0)
                a_angle += TWO_PI;
            while (a_angle > TWO_PI)
                a_angle -= TWO_PI;
            return a_angle;

            // return fmod(a_angle, TWO_PI) >= 0 ? a_angle : (a_angle + TWO_PI);
        }

        static float NormalRelativeAngle(float a_angle)
        {
            while (a_angle > PI)
                a_angle -= TWO_PI;
            while (a_angle < -PI)
                a_angle += TWO_PI;
            return a_angle;

            // return fmod(a_angle, TWO_PI) >= 0 ? (a_angle < PI) ? a_angle : a_angle - TWO_PI : (a_angle >= -PI) ? a_angle : a_angle + TWO_PI;
        }
    }; 
}
namespace ObjectUtil
{
    struct Transform
    {
        static void TranslateTo(RE::BSScript::IVirtualMachine *vm, RE::VMStackID stackID, RE::TESObjectREFR *object, float afX, float afY, float afZ, float afAngleX, float afAngleY, float afAngleZ, float afSpeed, float afMaxRotationSpeed)
        {
            using func_t = decltype(TranslateTo);
            REL::Relocation<func_t> func{RELOCATION_ID(55706, 56237)};
            func(vm, stackID, object, afX, afY, afZ, afAngleX, afAngleY, afAngleZ, afSpeed, afMaxRotationSpeed);
        }

        static float InterpAngleTo(float a_current, float a_target, float a_deltaTime, float a_interpSpeed)
        {
            if (a_interpSpeed <= 0.f)
            {
                return a_target;
            }

            const float distance = MathUtil::Angle::NormalRelativeAngle(a_target - a_current);

            if (distance * distance < FLT_EPSILON)
            {
                return a_target;
            }

            const float delta = distance * Clamp(a_deltaTime * a_interpSpeed, 0.f, 1.f);

            return a_current + delta;
        }

        static float Clamp(float value, float min, float max)
        {
            return value < min ? min : value < max ? value
                                                   : max;
        }
    };
}



namespace AnimUtil
{
    struct Idle
    {
            static bool Play(RE::TESIdleForm* idle, RE::Actor* actor, RE::DEFAULT_OBJECT action, RE::Actor* target)
            {
            if (actor && actor->GetActorRuntimeData().currentProcess)
            {
                typedef bool (*func_t)(RE::AIProcess *, RE::Actor *, RE::DEFAULT_OBJECT, RE::TESIdleForm *, bool, bool, RE::Actor *);
                REL::Relocation<func_t> func{RELOCATION_ID(38290, 39256)};
                return func(actor->GetActorRuntimeData().currentProcess, actor, action, idle, true, true, target);
            }
            return false;
            }
    };
}

namespace FormUtil
{
    struct Form
    {
            static std::string GetFormConfigString(TESForm* form) {
                if (!form) return "";
                
                std::string_view fileName = "Skyrim.esm";
                FormID formId = form->GetFormID();
                std::string hexFormId = fmt::format("0x{:X}", formId);
                if (form->IsDynamicForm()) {
                    return hexFormId;
                }

                // Try getting the local form id and the plugin that it belongs to
                try {
                    TESFileContainer fileContainer = form->sourceFiles;
                    if (fileContainer.array && fileContainer.array->size() > 0 && !fileContainer.array->empty()) {
                        TESFile* file = form->GetFile(0);
                        if (file) {
                            fileName = file->GetFilename();
                            formId = form->GetLocalFormID();
                            hexFormId = fmt::format("0x{:X}~{}", formId, fileName);
                        }
                    }
                } catch (...) {
                    // Do nothing
                }

                return hexFormId;
            } 

            static RE::TESForm *GetFormFromMod(std::string modname, uint32_t formid)
            {
            if (!modname.length() || !formid)
                return nullptr;
            RE::TESDataHandler *dh = RE::TESDataHandler::GetSingleton();
            RE::TESFile *modFile = nullptr;
            for (auto it = dh->files.begin(); it != dh->files.end(); ++it)
            {
                RE::TESFile *f = *it;
                if (strcmp(f->fileName, modname.c_str()) == 0)
                {
                    modFile = f;
                    break;
                }
            }
            if (!modFile)
                return nullptr;
            uint8_t modIndex = modFile->compileIndex;
            uint32_t id = formid;
            if (modIndex < 0xFE)
            {
                id |= ((uint32_t)modIndex) << 24;
            }
            else
            {
                uint16_t lightModIndex = modFile->smallFileCompileIndex;
                if (lightModIndex != 0xFFFF)
                {
                    id |= 0xFE000000 | (uint32_t(lightModIndex) << 12);
                }
            }
            return RE::TESForm::LookupByID(id);
            }

            static RE::TESForm *GetFormFromMod(std::string modname, std::string formIDString)
            {
                if (formIDString.length() == 0) return nullptr; 

                uint32_t formID = std::stoi(formIDString, 0, 16); 
                return GetFormFromMod(modname,formID); 
            } 

            static RE::TESForm *GetFormFromConfigString(std::string str, std::string_view delimiter)
            {
                std::vector<std::string> splitData = Util::String::Split(str, delimiter); 
                return GetFormFromMod(splitData[1], splitData[0]);
            }
            static RE::TESForm *GetFormFromConfigString(std::string str)
            {
                return GetFormFromConfigString(str, "~"sv); 
            }
    };
}
namespace NifUtil
{
    struct Node
		{
            static NiAVObject* Clone(NiAVObject* original)
            {
                typedef NiAVObject* (*func_t)(NiAVObject* avObj);
		        REL::Relocation<func_t> func{ RELOCATION_ID(68835, 70187) };
		        return func(original);
            }
			static RE::NiAVObject* GetNiObject(
				RE::NiNode*              a_root,
				const RE::BSFixedString& a_name)
			{
				return a_root->GetObjectByName(a_name);
			}

			static void AttachToNode(
				RE::NiAVObject* a_object,
				RE::NiNode*     a_node)
			{
				if (a_object->parent != a_node)
				{
					a_node->AttachChild(a_object, true);
				}
			}

            static std::vector<BSGeometry*> GetAllGeometries(RE::NiAVObject* root)
            {
                std::vector<BSGeometry*> geometries; 
                RE::BSVisit::TraverseScenegraphGeometries(root, [&](BSGeometry* geom)-> RE::BSVisit::BSVisitControl 
                {
                    geometries.emplace_back(geom); 
                    return RE::BSVisit::BSVisitControl::kContinue;
                }
				); 
                return geometries;
            }

		};
    struct Armature
    {
        static RE::NiNode* GetActorNode(RE::Actor* actor, std::string nodeName)
        {
                auto root = actor->Get3D();
                if (!root) return nullptr;

                auto bone = root->GetObjectByName(nodeName);
                if (!bone) return nullptr;

                auto node = bone->AsNode();
                if (!node) return nullptr;

                return node;
        }

        static void AttachToNode(RE::NiAVObject* obj, RE::Actor* actor, std::string nodeName)
        {
            auto* node = GetActorNode(actor, nodeName);
            if (node)
            {
                node->AttachChild(obj, true);
                SKSE::log::info("Object Attached");
            }
        }
    };
    struct Collision
    {
        static bool ToggleMeshCollision(RE::NiAVObject* root,RE::bhkWorld* world, bool collisionState)
        {
            constexpr auto no_collision_flag = static_cast<std::uint32_t>(RE::CFilter::Flag::kNoCollision);
					if (root && world) {
						
							RE::BSWriteLockGuard locker(world->worldLock);

							RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
								if (auto hkpBody = a_col->body ? static_cast<RE::hkpWorldObject*>(a_col->body->referencedObject.get()) : nullptr; hkpBody) {
									auto& filter = hkpBody->collidable.broadPhaseHandle.collisionFilterInfo;
									if (!collisionState) {
										filter |= no_collision_flag;
									} else {
										filter &= ~no_collision_flag;
									}
								}
								return RE::BSVisit::BSVisitControl::kContinue;
							});
					}
                    else 
                    {
                        return false;
                    }
            return true;
        }
         static bool RemoveMeshCollision(RE::NiAVObject* root,RE::bhkWorld* world, bool collisionState)
        {
            constexpr auto no_collision_flag = static_cast<std::uint32_t>(RE::CFilter::Flag::kNoCollision);
					if (root && world) {
						
							RE::BSWriteLockGuard locker(world->worldLock);

							RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
								if (auto hkpBody = a_col->body ? static_cast<RE::hkpWorldObject*>(a_col->body->referencedObject.get()) : nullptr; hkpBody) {
									auto& filter = hkpBody->collidable.broadPhaseHandle.collisionFilterInfo;
									if (!collisionState) {
										filter |= no_collision_flag;
									} else {
										filter &= ~no_collision_flag;
									}
								}
								return RE::BSVisit::BSVisitControl::kContinue;
							});
					}
                    else 
                    {
                        return false;
                    }
            return true;
        }
    };

    struct Effects
    {
        public:
        static BSTempEffectParticle* Spawn(TESObjectCELL* a_cell, float a_lifetime, const char* a_modelName, const NiPoint3& a_rotation, const NiPoint3& a_position, float a_scale, std::uint32_t a_flags, NiAVObject* a_target)
		{
			using func_t = BSTempEffectParticle* (*)(TESObjectCELL*, float, const char*, const NiPoint3&, const NiPoint3&, float, std::uint32_t, NiAVObject*);
			REL::Relocation<func_t> func{ RELOCATION_ID(29218, 30071) };
			return func(a_cell, a_lifetime, a_modelName, a_rotation, a_position, a_scale, a_flags, a_target);
		}

    };
}

namespace UIUtil { // Sourced from JunkIt
    struct ItemList {
        /**
         * @brief Get the Item List Menu object from ContainerMenu, BarterMenu, or InventoryMenu
         * 
         * @return RE::ItemList* 
         */
        static RE::ItemList* GetOpenList() {
            const auto UI = RE::UI::GetSingleton();
            RE::ItemList* itemListMenu = nullptr;

            if (!UI) {
                // UI is not available
                return nullptr;
            }

            if (UI->IsMenuOpen("ContainerMenu")) {
                itemListMenu = UI->GetMenu<ContainerMenu>()->GetRuntimeData().itemList;
            } else if (UI && UI->IsMenuOpen("BarterMenu")) { 
                itemListMenu = UI->GetMenu<BarterMenu>()->GetRuntimeData().itemList;
            } else if (UI && UI->IsMenuOpen("InventoryMenu")) { 
                itemListMenu = UI->GetMenu<InventoryMenu>()->GetRuntimeData().itemList;
            } else	{
                // None of the menus are open
                return nullptr;
            }

            if (!itemListMenu) {
                // Couldn't find an item list menu
                return nullptr;
            }

            return itemListMenu;
        }

        /**
         * @brief Refresh the Item List UI
         * 
         */
        static void Refresh() {
            RE::ItemList* itemListMenu = GetOpenList();
            if (itemListMenu) {
                itemListMenu->Update();
            }
        }
    };
    
    struct Menu {
        static RE::TESObjectREFR* GetBarterMenuTargetRef() {
            const auto UI = UI::GetSingleton();
            const auto menu = UI ? UI->GetMenu<BarterMenu>() : nullptr;
            if (!menu) return nullptr;

            const auto refHandle = menu->GetTargetRefHandle();
            TESObjectREFRPtr refr;

            // Invalid refHandle, this is the Player Reference's handle
            if (refHandle == 0x100000) { 
                uint32_t seAe_RefHandleMemAddress = 0x62840; // 0x62840 [(int32)403520] is the SE/AE memory address used in BarterMenu.h for the target ref handle
                uint32_t offset161170 = 0x70; // Manually found offset for Skyrim AE Version 1.6.1170
                
                REL::Relocation<RefHandle*> handle{ REL::ID(seAe_RefHandleMemAddress + offset161170) };
                const auto newRefHandle = *handle;
                LookupReferenceByHandle(newRefHandle, refr);
            } else {
                LookupReferenceByHandle(refHandle, refr);
            }

            if(!refr) {
                SKSE::log::error("===== ERROR could not determine the Vendor Actor for the BarterMenu");
                return nullptr;
            }

            return refr.get();
		}

        /**
         * @brief Get the Container Menu Mode
         *         kLoot = 0, kSteal = 1, kPickpocket = 2, kNPCMode = 3
         * @return ContainerMenu::ContainerMode 
         */
        static ContainerMenu::ContainerMode GetContainerMenuMode() {
            const auto UI = RE::UI::GetSingleton();
            const auto containerMenu = UI ? UI->GetMenu<ContainerMenu>() : nullptr;
            if (!containerMenu) {
                SKSE::log::info("No open menu found");
                return ContainerMenu::ContainerMode::kLoot;
            }

            ContainerMenu::ContainerMode mode = containerMenu->GetContainerMode();
            SKSE::log::info("Container Mode: {}", static_cast<std::uint32_t>(mode));
            return mode;
        }

        /**
         * @brief Get a UIs Menu Container
         *        Acceptable Template Types: ContainerMenu, BarterMenu, InventoryMenu
         * 
         * @return TESObjectREFR* 
         */
        template <typename T>
        static TESObjectREFR* GetContainer() {
            TESObjectREFR* actor = nullptr;
            const auto UI = RE::UI::GetSingleton();

            // If T is not a valid menu type, Look for an open menu's that use containers
            if (!std::is_base_of_v<RE::IMenu, T>) {
                return nullptr;
            }

            const auto menu = UI ? UI->GetMenu<T>() : nullptr;
            if (menu) {
                const auto refHandle = menu->GetTargetRefHandle();
                TESObjectREFRPtr refr;
                LookupReferenceByHandle(refHandle, refr);
                actor = refr.get();
            }

            return actor;
        }

        /**
         * @brief Get the Barter Menu Merchant Container object
         *        If the BarterMenu is open and the Vendor Actor 
         *        has a Vendor Faction, returns the Merchant Container
         * 
         * @return TESObjectREFR* 
         */
        static TESObjectREFR* GetMerchantContainer() {
            TESObjectREFR* vendorRef = GetContainer<BarterMenu>();
            if (!vendorRef) { 
                // This would really open happen if the function is called and the BarterMenu is not open
                return nullptr;
            }
            
            TESFaction* merchantFaction = vendorRef->As<Actor>()->GetVendorFaction();
            if (!merchantFaction) {
                // No merchant faction found - using vendor actor as merchantContainer
                return vendorRef;
            }
            
            // SKSE::log::info("Merchant faction found with id {:X} - using faction->merchantContainer", merchantFaction->GetFormID());
            TESObjectREFR* merchantContainer = merchantFaction->vendorData.merchantContainer;

            if (!merchantContainer) {
                // No merchant merchantContainer found for this actors merchant faction - using vendor actor as merchantContainer
                return vendorRef;
            }

            return merchantContainer;
        }
    };
}
