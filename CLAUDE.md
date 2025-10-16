# CLAUDE.md

## AI Assistant Name
**ARIA (아리아)** - **A**dvanced **R**esponsive **I**ntelligent **A**ssistant
고급 반응형 지능 어시스턴트

## Language Note

**중요: 이 프로젝트는 한국어로 개발이 진행됩니다.**
- 모든 코드 작업 시 한국어 댓글과 설명을 사용해 주세요
- 클로드와의 모든 대화는 한국어로 진행해 주세요
- 변수명과 함수명은 영어로 작성하되, 주석과 설명은 반드시 한국어로 작성해 주세요
- **번역 규칙**: "Kratos"는 한국어로 번역할 때 "크레토스"로 번역해 주세요 (크라토스 ❌)

**IMPORTANT: This project is developed in Korean.**
- Use Korean comments and explanations when working with code
- All conversations with Claude should be conducted in Korean
- Variable and function names should be in English, but comments and explanations must be in Korean

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a God of War Ragnarök-inspired action game built in Unreal Engine 5.5. The project implements a combat system featuring Kratos with his Leviathan Axe, various enemy types, and gameplay abilities using UE5's Gameplay Ability System (GAS).

## Build Commands

Since this is an Unreal Engine project, building is typically done through the Unreal Editor or Visual Studio:

- **Open in Editor**: Launch `Ragnarok.uproject` with Unreal Engine 5.5
- **Generate Project Files**: Right-click on `Ragnarok.uproject` → "Generate Visual Studio project files"
- **Build in Visual Studio**: Open `Ragnarok.sln` and build the solution
- **Package Game**: Use Unreal Editor's File → Package Project menu

## Code Architecture

### Module Structure
- **Ragnarok**: Main game module containing all game-specific code
- **Engine Version**: Unreal Engine 5.5
- **Key Dependencies**: GameplayAbilities, EnhancedInput, MotionWarping, Niagara

### Core Systems Architecture

**RagnarokEngine**: Foundation layer providing base classes and systems
- `Core/`: Base character, game framework, animation, and component classes
- `Systems/`: Modular systems (AbilitySystem, CombatSystem, EnemySystem, etc.)
- `Objects/`: Reusable game objects like weapons

**RagnarokContent**: Game-specific implementations
- `Characters/Kratos/`: Player character implementation with combat, abilities, and weapons
- `Characters/Enemy/`: Enemy implementations (Gruntling, Guardian, etc.)
- `Abilities/`: Gameplay abilities using UE5's GAS

### Key Systems

**Gameplay Ability System (GAS)**:
- `RagnarokAbilitySystemComponent`: Core ASC implementation
- `RagnarokGameplayAbility`: Base ability class
- Character-specific abilities in respective folders (Kratos, Enemy)

**Combat System**:
- `CombatComponent`: Handles combat logic and interactions
- `CombatInterface`: Interface for combat-capable actors
- Weapon collision through animation notifies

**AI System**:
- Behavior Tree decorators, services, and task nodes
- `EnemyAIController`: AI controller for enemy characters
- Motion warping integration for attack targeting

**Input System**:
- Enhanced Input integration
- `InputConfigDataAsset`: Configurable input mappings

### Character Architecture

**Base Classes**:
- `RagnarokCharacter`: Base character with GAS integration
- `CharacterAnimInstance`: Base animation blueprint class

**Kratos Implementation**:
- `Kratos`: Main player character class
- `KratosAnimInstance`: Animation logic
- `KratosCombatComponent`: Combat-specific functionality
- `KratosWeapon`/`LeviathanAxe`: Weapon implementations

**Enemy System**:
- `EnemyCharacter`: Base enemy class
- `EnemyAIController`: AI behavior
- `EnemyCombatComponent`: Enemy combat logic

### Asset Management
- `RagnarokAssetManager`: Custom asset manager
- `RagnarokPrimaryDataAsset`: Base class for primary data assets
- Character and item data assets for configuration

## Development Notes

- The project uses a modular architecture separating engine-level systems from content-specific implementations
- Heavy use of UE5's Gameplay Ability System for character abilities and effects
- Animation-driven combat with extensive use of Animation Notifies
- Behavior Tree-based AI with custom decorators and task nodes
- Enhanced Input system for player controls

## Git Commit Guidelines

### Commit Message Format
Use the following prefixes for commit messages:

- **feat**: 새로운 기능 추가, 기존의 기능을 요구 사항에 맞추어 수정
- **fix**: 기능에 대한 버그 수정
- **build**: 빌드 관련 수정
- **chore**: 패키지 매니저 수정, 그 외 기타 수정 ex) .gitignore
- **ci**: CI 관련 설정 수정
- **docs**: 문서(주석) 수정
- **style**: 코드 스타일, 포맷팅에 대한 수정
- **refactor**: 기능의 변화가 아닌 코드 리팩터링 ex) 변수 이름 변경
- **test**: 테스트 코드 추가/수정
- **release**: 버전 릴리즈

### Commit Message Writing Style
When writing commit messages, use a **casual, direct, and natural tone** rather than formal or AI-like language:

**Good Examples:**
```
feat: add running state check to prevent normal attacks while running

Attack Ability Conditions:
- Add CanActivateAbility to heavy/light attack abilities
- Block normal attacks when Kratos is running
- Only allow attacks when standing still or walking

Why this change:
- Need to separate regular attacks from dash attacks
- Regular attacks shouldn't work while running
- Creates proper attack state differentiation
```

```
feat: implement dash heavy attack movement system

- Add BeginMovement(), TickMovement(), EndMovement() functions for dash movement
- Implement smooth interpolation between start and target positions  
- Sync movement duration with animation montage playback time

대시 강공격 이동 시스템 구현:
- BeginMovement(), TickMovement(), EndMovement() 함수로 대시 이동 처리
- 시작점과 목표점 간 부드러운 보간 이동 구현
- 애니메이션 몽타주 재생 시간과 이동 시간 동기화
```

```
feat: implement AnimNotifyState movement system for character animations

Movement System Implementation:
- Add LinearMovementAnimNotifyState for straight-line movement control
- Add ArcMovementAnimNotifyState foundation for jump/arc movements  
- Support 8-directional movement with custom direction option
- Include speed curve support for non-linear movement patterns
- Disable code-based movement in dash heavy attack for AnimNotify testing

애니메이션 기반 이동 시스템 구현:
- 직선 이동용 LinearMovementAnimNotifyState 추가
- 점프/호형 이동용 ArcMovementAnimNotifyState 기초 구현
- 8방향 이동 및 커스텀 방향 지원
- 비선형 이동 패턴을 위한 속도 커브 지원
- AnimNotify 테스트를 위해 대시 강공격의 코드 기반 이동 비활성화
```

```
feat: complete ArcMovementAnimNotifyState implementation and refactor movement types

Arc Movement System Implementation:
- Implement complete ArcMovementAnimNotifyState with parabolic motion calculation
- Add configurable Duration property for independent arc movement timing  
- Create EArcMovementType enum (Jump, Leap, Parabolic, Custom) in RagnarokContentTypes
- Refactor LinearMovementAnimNotifyState to use shared ERagnarokDirection enum
- Add proper Super::NotifyEnd() calls for consistent cleanup
- Update animation montage with ArcMovementAnimNotifyState integration

호형 이동 시스템 완성 및 이동 타입 리팩터링:
- 포물선 운동 계산을 포함한 ArcMovementAnimNotifyState 완전 구현
- 독립적인 호형 이동 타이밍을 위한 Duration 속성 추가
- RagnarokContentTypes에 EArcMovementType 열거형 생성 (점프, 도약, 포물선, 커스텀)
- 공유 ERagnarokDirection 열거형 사용으로 LinearMovementAnimNotifyState 리팩터링  
- 일관된 정리를 위한 Super::NotifyEnd() 호출 추가
- ArcMovementAnimNotifyState 통합으로 애니메이션 몽타주 업데이트
```

```
feat: implement dash light attack system with animation and ability integration

Dash Light Attack Implementation:
- Add KratosDashLightAttackAbility class
- Create new animation asset: KratosDashLightAttack.uasset
- Add gameplay ability blueprint: GA_KratosDashLightAttackAxe.uasset
- Create animation montage: AM_KratosDashLightAttack.uasset
- Add dash light attack ability to BP_LeviathanAxe owned abilities

대시 약공격 시스템 구현:
- KratosDashLightAttackAbility 클래스 추가
- 새 애니메이션 에셋 생성: KratosDashLightAttack.uasset
- 게임플레이 어빌리티 블루프린트 추가: GA_KratosDashLightAttackAxe.uasset
- 애니메이션 몽타주 생성: AM_KratosDashLightAttack.uasset
- BP_LeviathanAxe 소유 어빌리티에 대시 약공격 어빌리티 추가
```

```
feat: add Leviathan Axe mesh and material assets

Leviathan Axe Asset Addition:
- Import Leviathon Axe assets
- Update BP_LeviathanAxe.uasset blueprint
- Update PDA_LeviathanAxe.uasset initial skeletal mesh

리바이어던 도끼 메시 및 머티리얼 에셋 추가:
- Leviathon Axe 에셋 임포트
- BP_LeviathanAxe.uasset 블루프린트 업데이트
- PDA_LeviathanAxe.uasset 초기화 스켈레탈 메시 업데이트
```

```
feat: implement weapon catch system foundation with event-driven gameplay ability pattern

Weapon Catch System Foundation:
- Add KratosCatchWeaponAbility class with basic function implementation
- Create catch animation playback, camera shake, and weapon attachment functions
- Add GA_KratosCatchWeapon gameplay ability blueprint asset
- Add GA_KratosCatchWeapon to DA_KratosStartUp data asset

무기 캐치 시스템 기초 구현:
- KratosCatchWeaponAbility 클래스 추가 및 기초 함수 구현
- 캐치 애니메이션 재생, 카메라 흔들림, 무기 부착 함수 생성
- GA_KratosCatchWeapon 게임플레이 어빌리티 블루프린트 에셋 추가
- DA_KratosStartUp 데이터 에셋에 GA_KratosCatchWeapon 추가
```

**Avoid:**
- Overly formal language like "Implementation of comprehensive system for..."
- AI-like phrases such as "This enhancement provides..."
- Verbose academic writing style

**Preferred tone:**
- Direct and conversational
- Focus on what was changed and why
- Use bullet points for clarity
- Include both English and Korean explanations when helpful
- Keep descriptions concise and to the point
- **IMPORTANT**: Keep commit messages shorter and simpler - avoid overly detailed explanations

### Important Git Rules
- **NEVER commit automatically**: When asked to write a commit message, only provide the message text - do not execute the actual commit
- Always write commit messages in English (title only)
- For detailed descriptions, provide both English and Korean versions
- **CRITICAL: Use the appropriate prefix from the list above** - Be very careful about prefix selection:
  - **feat**: Only for new features or modifying existing features to meet new requirements
  - **fix**: Only for actual bug fixes
  - **chore**: Only for package manager changes, .gitignore, etc.
  - **refactor**: Only for code restructuring without functional changes
  - When in doubt, carefully consider what type of change it actually is
- Follow the casual, natural writing style described above
- **CRITICAL: Bilingual Consistency**: When providing both English and Korean descriptions, they must convey the EXACT SAME content. The Korean section should be a direct translation of the English content, not different or additional information. Both sections should describe identical changes and implementation details.

### Commit Message Accuracy Guidelines
When writing commit messages, use precise and accurate action verbs that match the actual changes:

**Function/Method Changes:**
- **구현/Implement**: For newly created functions, methods, or classes
- **추가/Add**: For new variables, properties, states, or enums
- **수정/Modify** or **업데이트/Update**: For changes to existing functionality
- **통합/Integrate**: For connecting or combining existing systems

**Example of proper usage:**
```
리바이어던 도끼 벽 박힘 시스템 구현:
- 표면 기반 위치 및 회전용 LodgeAxe() 함수 구현
- 현실적인 도끼 각도 계산용 CalcAxeImpactPitch() 구현
- Z 오프셋으로 정확한 박힘 위치용 CalcAxeImactLocation() 구현
- 이중 사운드 큐 충돌 오디오용 PlayHitSoundCue() 구현
- ERagnarokWeaponState 열거형에 ERWS_Lodge 상태 추가
- EndWeaponTrail() 및 사운드 이펙트와 충돌 감지 통합

Axe Lodging System Implementation:
- Implement LodgeAxe() function for surface-based positioning and rotation
- Implement CalcAxeImpactPitch() for realistic axe angle calculation
- Implement CalcAxeImactLocation() for accurate lodging position with Z-offset
- Implement PlayHitSoundCue() for dual sound cue impact audio
- Add ERWS_Lodge state to ERagnarokWeaponState enum
- Integrate EndWeaponTrail() and sound effects with collision detection
```

**Important**: Always use the correct verb that matches the actual change type - don't use "추가/Add" for function implementation or "구현/Implement" for adding simple properties.

## Header File Documentation Guidelines

### 언리얼 엔진 스타일 헤더 파일 주석 작성 가이드라인

When asked to add comments to header files (.h files), follow these comprehensive guidelines:

### 1. 주석 스타일 규칙 (Comment Style Rules)

**함수 설명 형식**:
- ❌ "뭐뭐 한다" (does something)  
- ✅ "뭐뭐 하는 함수" (function that does something)

**여러 줄 주석 형식**:
```cpp
// ❌ 잘못된 예
/** 함수 설명이 바로 시작됨 */

// ✅ 올바른 예  
/**
 * 함수 설명이 개행 후 시작됨
 */
```

**매개변수 주석 간격**:
```cpp
/**
 * 함수 설명
 *
 * @param Parameter1 매개변수1 설명
 * @param Parameter2 매개변수2 설명
 */
```

### 2. 클래스 주석 (Class Comments)

```cpp
/**
 * 한국어 클래스 설명
 * 클래스의 목적과 주요 기능을 한국어로 설명
 * 
 * English class description
 * Purpose and main functionality in English
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class RAGNAROK_API AClassName : public ABaseClass
```

### 3. 함수 주석 (Function Comments)

**단일 줄 주석 (매개변수 없음)**:
```cpp
/** 게임 시작 시 호출됨 */
virtual void BeginPlay() override;

/** VFX 컴포넌트를 초기화하는 함수 */
void InitVFX();
```

**다중 줄 주석 (매개변수 있음)**:
```cpp
/**
 * 무기를 던지는 함수
 * 카메라 방향으로 도끼를 던지며 회전과 추적 타임라인을 시작하는 함수
 *
 * @param CameraRotation 카메라 회전값
 * @param CameraLocation 카메라 위치  
 * @param CameraForwardVector 카메라 전방 벡터 (던지기 방향)
 */
virtual void ThrowWeapon(FRotator CameraRotation, FVector CameraLocation, FVector CameraForwardVector) override;
```

**상속받은 virtual 함수도 구체적으로 설명**:
```cpp
/**
 * 무기 프라이머리 데이터 에셋을 비동기로 로드하는 함수
 * 리바이어던 도끼의 메시와 관련 데이터를 비동기로 로드하는 함수
 *
 * @param PDAAssetObject 로드할 프라이머리 데이터 에셋 오브젝트
 */
virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
```

### 4. 변수 주석 (Variable Comments)

**UPROPERTY 변수**:
```cpp
/** 도끼 회전의 중심점이 되는 컴포넌트 */
UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Components", meta = (DisplayName = "Pivot Point"))
USceneComponent* PivotPointComponent = nullptr;

/** 던지기 사운드 맵 (true/false에 따른 다른 사운드) */
UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Throw Sound Map"))
TMap<bool, USoundBase*> ThrowSoundMap;
```

**일반 private 변수**:
```cpp
/** 피벗 초기 위치 */
FVector PivotInitLocation = FVector::ZeroVector;

/** 던지기 거리 */
float ThrowDistance = 250.0f;

/** 던지기 사운드 플립플롭 상태 */
bool bThrowSoundFlipFlop = true;
```

### 5. UPROPERTY 메타데이터 (UPROPERTY Metadata)

**카테고리 분류**:
- `Category = "LeviathanAxe|Components"` (컴포넌트)
- `Category = "LeviathanAxe|Audio"` (오디오)  
- `Category = "LeviathanAxe|VFX"` (비주얼 이펙트)
- `Category = "LeviathanAxe|Animation Curves"` (애니메이션 커브)
- `Category = "LeviathanAxe|Recall"` (회수 관련)

**메타 속성**:
- `meta = (DisplayName = "User Friendly Name")` (에디터 표시명)
- `BlueprintReadOnly` (블루프린트 읽기 전용)
- `BlueprintReadWrite` (블루프린트 읽기/쓰기)

### 6. 특별 주의사항 (Special Considerations)

**CPP 파일 분석이 필요한 경우**:
- 매개변수가 있는 사용자 정의 함수들의 경우, CPP 파일을 분석해서 실제 용도를 파악한 후 정확한 주석 작성
- 실제로 사용되지 않는 매개변수는 명시: `@param UnusedParam 사용되지 않는 매개변수`

**함수 종류별 설명**:
- 초기화 함수: "~를 초기화하는 함수"
- 계산 함수: "~를 계산하는 함수"  
- 재생 함수: "~를 재생하는 함수"
- 설정 함수: "~를 설정하는 함수"
- 콜백 함수: "~ 시 호출되는 함수"

### 7. 실제 적용 예시

아래는 완전한 헤더 파일 주석 처리 예시입니다:

```cpp
/**
 * 리바이어던 도끼 클래스  
 * 크레토스의 주무기로, 던지기/회수/박힘 등의 기능을 가진 무기
 * 
 * Leviathan Axe class.
 * Kratos' primary weapon with throwing, recalling, and lodging capabilities.
 */
UCLASS()
class RAGNAROK_API ALeviathanAxe : public AKratosWeapon
{
    GENERATED_BODY()

public:
    /** 생성자 */
    ALeviathanAxe();

    /**
     * 무기를 던지는 함수
     * 카메라 방향으로 도끼를 던지며 회전과 추적 타임라인을 시작하는 함수
     *
     * @param CameraRotation 카메라 회전값
     * @param CameraLocation 카메라 위치
     * @param CameraForwardVector 카메라 전방 벡터 (던지기 방향)
     */
    virtual void ThrowWeapon(FRotator CameraRotation, FVector CameraLocation, FVector CameraForwardVector) override;

protected:
    /** 도끼 회전의 중심점이 되는 컴포넌트 */
    UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Components", meta = (DisplayName = "Pivot Point"))
    USceneComponent* PivotPointComponent = nullptr;

private:
    /** 던지기 거리 */
    float ThrowDistance = 250.0f;
};
```

### 8. 주석 작성 체크리스트

헤더 파일에 주석을 추가할 때 다음 사항들을 확인하세요:

- [ ] 클래스에 한국어/영어 설명 추가
- [ ] 모든 public/protected 함수에 주석 추가
- [ ] 매개변수 있는 함수에 @param 주석 추가  
- [ ] 함수 설명과 @param 사이에 빈 줄 추가
- [ ] "~하는 함수" 형식으로 함수 설명 작성
- [ ] UPROPERTY에 Category와 DisplayName 메타데이터 추가
- [ ] 모든 변수에 용도 설명 추가
- [ ] CPP 파일 분석이 필요한 복잡한 함수는 구현 확인 후 정확한 주석 작성

이 가이드라인을 따라 언리얼 엔진의 전문적인 코딩 스타일에 맞는 주석을 작성하세요.

## Header File Restructuring Guidelines

### 언리얼 엔진 스타일 헤더 파일 재구조화 가이드라인

When asked to restructure header files (.h files), follow these Epic Games coding standards:

### 1. 접근 지정자 순서 (Access Specifier Order)

Always organize class members in the following order:

```cpp
public:     // 1순위 - 외부에서 사용하는 인터페이스
protected:  // 2순위 - 상속 클래스에서 사용
private:    // 3순위 - 내부 구현
```

### 2. 각 섹션 내 구성 요소 순서 (Order Within Each Section)

#### Public Section:
```cpp
public:
    // 1. 생성자/소멸자 (Constructors/Destructors)
    /** 생성자 */
    AClassName();

    // 2. Interface 오버라이드 (Interface Overrides)
    //~ Begin AActor Interface.
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    //~ End AActor Interface.

    // 3. Public 함수들 (Public Functions)
    // (주석으로 그룹 분리)

    // 4. Inline 함수들 - 주로 Getter/Setter (Inline Functions)
    // Getters (Inline Functions)
    /** 설명 */
    FORCEINLINE Type GetValue() const { return Value; }

    // 5. Public 변수들 (Public Variables)
    // Public Variables (BlueprintAssignable Delegates)
    /** 설명 */
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FDelegateType OnEvent;
```

#### Protected Section:
```cpp
protected:
    // 1. Virtual 함수들 (Virtual Functions)
    // Protected Functions

    // 2. UFUNCTION() 콜백들 (UFUNCTION Callbacks)

    // 3. 일반 Protected 함수들 (Regular Protected Functions)

    // 4. UPROPERTY 변수들 (UPROPERTY Variables)
    // Protected Variables (UPROPERTY for Editor/Blueprint access)

    // 5. Non-UPROPERTY 변수들 (Non-UPROPERTY Variables)
    // Protected Variables (Non-UPROPERTY)
```

#### Private Section:
```cpp
private:
    // 1. Private 함수들 (Private Functions)
    // Private Functions (Internal implementation only)

    // 2. Private 변수들 (Private Variables)
    // Private Variables (Internal state only)
```

### 3. UPROPERTY 변수 그룹핑 (UPROPERTY Variable Grouping)

UPROPERTY 변수들은 논리적으로 그룹화하고 다음 순서로 배치:

```cpp
// 1. Components 먼저 (Components First)
/** 컴포넌트 설명 */
UPROPERTY(EditAnywhere, Category = "ClassName|Components", meta = (DisplayName = "Component Name"))
UComponentType* ComponentName = nullptr;

// 2. Data/Config 변수들 (Data/Config Variables)
/** 데이터 설명 */
UPROPERTY(EditAnywhere, Category = "ClassName|Data", meta = (DisplayName = "Data Name"))
FDataType DataName;

// 3. Non-UPROPERTY 변수는 마지막 (Non-UPROPERTY Variables Last)
/** 내부 상태 변수 설명 */
bool bInternalState = false;
```

### 4. 주석을 이용한 논리적 그룹핑 (Logical Grouping with Comments)

각 섹션 내에서 관련 기능별로 주석을 사용해 그룹화:

```cpp
public:
    // Public Functions
    virtual void SomeFunction() override;

    // Getters (Inline Functions)
    FORCEINLINE Type GetValue() const { return Value; }

    // Public Variables (BlueprintAssignable Delegates)
    UPROPERTY(BlueprintAssignable)
    FDelegateType OnEvent;

protected:
    // Protected Functions
    virtual void ProtectedFunction();

    // Protected Variables (UPROPERTY for Editor/Blueprint access)
    UPROPERTY(EditAnywhere, Category = "Components")
    UComponent* Component = nullptr;

    // Protected Variables (Non-UPROPERTY)
    bool bProtectedFlag = false;
```

### 5. 실제 재구조화 예시

#### Before (잘못된 구조):
```cpp
UCLASS()
class AWeapon : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnEvent OnEvent;

    FORCEINLINE UMeshComponent* GetMesh() const { return Mesh; }

    AWeapon();

protected:
    UPROPERTY(EditAnywhere)
    UMeshComponent* Mesh = nullptr;

    virtual void BeginPlay() override;

    bool bInitialized = false;

private:
    void InternalFunction();

public:
    virtual void SomeFunction() override;
};
```

#### After (올바른 구조):
```cpp
UCLASS()
class AWeapon : public AActor
{
    GENERATED_BODY()

public:
    /** 생성자 */
    AWeapon();

    //~ Begin AActor Interface.
    virtual void BeginPlay() override;
    //~ End AActor Interface.

    // Public Functions
    virtual void SomeFunction() override;

    // Getters (Inline Functions)
    /** 메시 컴포넌트를 반환하는 함수 */
    FORCEINLINE UMeshComponent* GetMesh() const { return Mesh; }

    // Public Variables (BlueprintAssignable Delegates)
    /** 이벤트 델리게이트 */
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEvent OnEvent;

protected:
    // Protected Variables (UPROPERTY for Editor/Blueprint access)
    /** 메시 컴포넌트 */
    UPROPERTY(EditAnywhere, Category = "Components", meta = (DisplayName = "Mesh Component"))
    UMeshComponent* Mesh = nullptr;

    // Protected Variables (Non-UPROPERTY)
    /** 초기화 완료 여부 */
    bool bInitialized = false;

private:
    // Private Functions (Internal implementation only)
    void InternalFunction();
};
```

### 6. 재구조화 체크리스트

헤더 파일을 재구조화할 때 다음 사항들을 확인하세요:

- [ ] 접근 지정자가 public → protected → private 순서로 정렬되어 있는가?
- [ ] 각 섹션 내에서 생성자 → Interface 오버라이드 → 함수들 → Inline 함수들 → 변수들 순서로 정렬되어 있는가?
- [ ] 논리적 그룹별로 주석이 추가되어 있는가?
- [ ] UPROPERTY 변수들이 Components → Data → Non-UPROPERTY 순서로 배치되어 있는가?
- [ ] Interface 오버라이드가 Begin/End 주석으로 구분되어 있는가?
- [ ] Inline 함수들이 별도 그룹으로 분리되어 있는가?
- [ ] BlueprintAssignable 델리게이트들이 Public 변수 그룹에 있는가?

### 7. UCLASS 메타데이터 처리

재구조화 시 UCLASS 메타데이터는 요청에 따라 처리:
- 명시적 요청이 없는 한 기존 UCLASS() 유지
- "UCLASS 비워달라"는 요청 시에만 `UCLASS()` 형태로 변경

이 가이드라인을 따라 Epic Games 스타일의 깔끔하고 일관된 헤더 파일 구조를 만드세요.