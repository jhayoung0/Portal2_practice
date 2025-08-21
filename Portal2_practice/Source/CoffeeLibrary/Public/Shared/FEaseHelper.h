#pragma once

UENUM(BlueprintType)
enum class EEaseType : uint8
{
    Linear           UMETA(DisplayName="Linear"),
    EaseInQuad       UMETA(DisplayName="EaseInQuad"),
    EaseOutQuad      UMETA(DisplayName="EaseOutQuad"),
    EaseInOutQuad    UMETA(DisplayName="EaseInOutQuad"),
    EaseInCubic      UMETA(DisplayName="EaseInCubic"),
    EaseOutCubic     UMETA(DisplayName="EaseOutCubic"),
    EaseInOutCubic   UMETA(DisplayName="EaseInOutCubic"),
    EaseInQuart      UMETA(DisplayName="EaseInQuart"),
    EaseOutQuart     UMETA(DisplayName="EaseOutQuart"),
    EaseInOutQuart   UMETA(DisplayName="EaseInOutQuart"),
    EaseInQuint      UMETA(DisplayName="EaseInQuint"),
    EaseOutQuint     UMETA(DisplayName="EaseOutQuint"),
    EaseInOutQuint   UMETA(DisplayName="EaseInOutQuint"),
    EaseInSine       UMETA(DisplayName="EaseInSine"),
    EaseOutSine      UMETA(DisplayName="EaseOutSine"),
    EaseInOutSine    UMETA(DisplayName="EaseInOutSine"),
    EaseInExpo       UMETA(DisplayName="EaseInExpo"),
    EaseOutExpo      UMETA(DisplayName="EaseOutExpo"),
    EaseInOutExpo    UMETA(DisplayName="EaseInOutExpo"),
    EaseInCirc       UMETA(DisplayName="EaseInCirc"),
    EaseOutCirc      UMETA(DisplayName="EaseOutCirc"),
    EaseInOutCirc    UMETA(DisplayName="EaseInOutCirc"),
    EaseInElastic    UMETA(DisplayName="EaseInElastic"),
    EaseOutElastic   UMETA(DisplayName="EaseOutElastic"),
    EaseInOutElastic UMETA(DisplayName="EaseInOutElastic"),
    EaseInBack       UMETA(DisplayName="EaseInBack"),
    EaseOutBack      UMETA(DisplayName="EaseOutBack"),
    EaseInOutBack    UMETA(DisplayName="EaseInOutBack"),
    EaseInBounce     UMETA(DisplayName="EaseInBounce"),
    EaseOutBounce    UMETA(DisplayName="EaseOutBounce"),
    EaseInOutBounce  UMETA(DisplayName="EaseInOutBounce")
};

struct FEaseHelper
{
    static float Ease(float Alpha, EEaseType Type)
    {
        Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

        switch (Type)
        {
            case EEaseType::Linear:           return Alpha;
            case EEaseType::EaseInQuad:       return Alpha * Alpha;
            case EEaseType::EaseOutQuad:      return 1.f - FMath::Pow(1.f - Alpha, 2.f);
            case EEaseType::EaseInOutQuad:    return Alpha < 0.5f ? 2.f*Alpha*Alpha : 1.f - FMath::Pow(-2.f*Alpha+2.f,2.f)/2.f;
            case EEaseType::EaseInCubic:      return Alpha*Alpha*Alpha;
            case EEaseType::EaseOutCubic:     return 1.f - FMath::Pow(1.f - Alpha, 3.f);
            case EEaseType::EaseInOutCubic:   return Alpha < 0.5f ? 4.f*Alpha*Alpha*Alpha : 1.f - FMath::Pow(-2.f*Alpha+2.f,3.f)/2.f;
            case EEaseType::EaseInQuart:      return Alpha*Alpha*Alpha*Alpha;
            case EEaseType::EaseOutQuart:     return 1.f - FMath::Pow(1.f - Alpha, 4.f);
            case EEaseType::EaseInOutQuart:   return Alpha < 0.5f ? 8.f*Alpha*Alpha*Alpha*Alpha : 1.f - FMath::Pow(-2.f*Alpha+2.f,4.f)/2.f;
            case EEaseType::EaseInQuint:      return Alpha*Alpha*Alpha*Alpha*Alpha;
            case EEaseType::EaseOutQuint:     return 1.f - FMath::Pow(1.f - Alpha, 5.f);
            case EEaseType::EaseInOutQuint:   return Alpha < 0.5f ? 16.f*FMath::Pow(Alpha,5.f) : 1.f - FMath::Pow(-2.f*Alpha+2.f,5.f)/2.f;
            case EEaseType::EaseInSine:       return 1.f - FMath::Cos((Alpha * PI) / 2.f);
            case EEaseType::EaseOutSine:      return FMath::Sin((Alpha * PI) / 2.f);
            case EEaseType::EaseInOutSine:    return -(FMath::Cos(PI*Alpha) - 1.f)/2.f;
            case EEaseType::EaseInExpo:       return Alpha == 0.f ? 0.f : FMath::Pow(2.f, 10.f*Alpha - 10.f);
            case EEaseType::EaseOutExpo:      return Alpha == 1.f ? 1.f : 1.f - FMath::Pow(2.f, -10.f*Alpha);
            case EEaseType::EaseInOutExpo:    return Alpha == 0.f ? 0.f : Alpha == 1.f ? 1.f : Alpha < 0.5f ? FMath::Pow(2.f, 20.f*Alpha - 10.f)/2.f : (2.f - FMath::Pow(2.f,-20.f*Alpha + 10.f))/2.f;
            case EEaseType::EaseInCirc:       return 1.f - FMath::Sqrt(1.f - FMath::Pow(Alpha,2.f));
            case EEaseType::EaseOutCirc:      return FMath::Sqrt(1.f - FMath::Pow(Alpha-1.f,2.f));
            case EEaseType::EaseInOutCirc:    return Alpha < 0.5f ? (1.f - FMath::Sqrt(1.f - FMath::Pow(2.f*Alpha,2.f)))/2.f : (FMath::Sqrt(1.f - FMath::Pow(-2.f*Alpha+2.f,2.f))+1.f)/2.f;

            case EEaseType::EaseInElastic:     return Alpha == 0.f ? 0.f : Alpha == 1.f ? 1.f : -FMath::Pow(2.f, 10.f*Alpha - 10.f) * FMath::Sin((Alpha*10.f - 10.75f) * (2.f*PI/3.f));
            case EEaseType::EaseOutElastic:    return Alpha == 0.f ? 0.f : Alpha == 1.f ? 1.f : FMath::Pow(2.f, -10.f*Alpha) * FMath::Sin((Alpha*10.f - 0.75f) * (2.f*PI/3.f)) + 1.f;
            case EEaseType::EaseInOutElastic:  return Alpha == 0.f ? 0.f : Alpha == 1.f ? 1.f : Alpha < 0.5f ? -(FMath::Pow(2.f, 20.f*Alpha-10.f) * FMath::Sin((20.f*Alpha-11.125f)*(2.f*PI/4.5f)))/2.f : (FMath::Pow(2.f,-20.f*Alpha+10.f)*FMath::Sin((20.f*Alpha-11.125f)*(2.f*PI/4.5f)))/2.f + 1.f;

            case EEaseType::EaseInBack:
            {
                constexpr float c1 = 1.70158f;
                return Alpha*Alpha*((c1+1.f)*Alpha - c1);
            }
            case EEaseType::EaseOutBack:
            {
                constexpr float c1 = 1.70158f;
                constexpr float c3 = c1 + 1.f;
                return 1.f + (Alpha-1.f)*(Alpha-1.f)*(c3*(Alpha-1.f) + c1);
            }
            case EEaseType::EaseInOutBack:
            {
                constexpr float c1 = 1.70158f;
                constexpr float c2 = c1*1.525f;
                return Alpha < 0.5f ? (FMath::Pow(2.f*Alpha,2.f)*((c2+1.f)*2.f*Alpha - c2))/2.f : (FMath::Pow(2.f*Alpha-2.f,2.f)*((c2+1.f)*(Alpha*2.f-2.f)+c2)+2.f)/2.f;
            }

            case EEaseType::EaseInBounce:   return 1.f - Ease(1.f - Alpha, EEaseType::EaseOutBounce);
            case EEaseType::EaseOutBounce:
            {
                constexpr float n1 = 7.5625f;
                constexpr float d1 = 2.75f;
                float x = Alpha;
                if (x < 1.f/d1) return n1*x*x;
                else if (x < 2.f/d1) return n1*(x-=1.5f/d1)*x + 0.75f;
                else if (x < 2.5f/d1) return n1*(x-=2.25f/d1)*x + 0.9375f;
                else return n1*(x-=2.625f/d1)*x + 0.984375f;
            }
            case EEaseType::EaseInOutBounce: return Alpha < 0.5f ? (1.f - Ease(1.f - 2.f*Alpha, EEaseType::EaseOutBounce))/2.f : (1.f + Ease(2.f*Alpha-1.f, EEaseType::EaseOutBounce))/2.f;

            default: return Alpha;
        }
    }
};
