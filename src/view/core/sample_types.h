#ifndef _SAMPLE_TYPES_H_
#define _SAMPLE_TYPES_H_

#include <stdint.h>
#include <ippdefs.h>
#include <assert.h>

namespace dsp
{

enum SampleType {
    kSample16s      = 1,
    kSample16sc     = 2,
    kSample32f      = 3,
    kSample32fc     = 4,
    kSample64f      = 5,
    kSample64fc     = 6,
    kSample12s      = 7,
    kSample12sc     = 8,
    kSample32sc     = 9
};

template<SampleType st>
struct SampleTraits;

template<>
struct SampleTraits<kSample12s>
{
    typedef int16_t SampleType;

    enum { is_complex   = false         };
    enum { is_integral  = true          };
    enum { NextWideType = kSample32f    };
    enum { size = 2                     };
};

template<>
struct SampleTraits<kSample12sc>
{
    typedef Ipp16sc SampleType;

    enum { is_complex   = true          };
    enum { is_integral  = true          };
    enum { NextWideType = kSample32fc   };
    enum { size = 3                     };
};

template<>
struct SampleTraits<kSample16s>
{
    typedef int16_t SampleType;

    enum { is_complex   = false         };
    enum { is_integral  = true          };
    enum { NextWideType = kSample32f    };
    enum { size = sizeof(SampleType)    };
};

template<>
struct SampleTraits<kSample16sc>
{
    typedef Ipp16sc SampleType;

    enum { is_complex   = true          };
    enum { is_integral  = true          };
    enum { NextWideType = kSample32fc   };
    enum { size = sizeof(SampleType)    };
};

template<>
struct SampleTraits<kSample32f>
{
    typedef Ipp32f SampleType;

    enum { is_complex   = false         };
    enum { is_integral  = false         };
    enum { NextWideType = kSample64f    };
    enum { size = sizeof(SampleType)    };
};

template<>
struct SampleTraits<kSample32fc>
{
    typedef Ipp32fc SampleType;
    enum { is_complex   = true          };
    enum { is_integral  = false         };
    enum { NextWideType = kSample64fc   };
    enum { size = sizeof(SampleType)    };
};

template<>
struct SampleTraits<kSample64f>
{
    typedef Ipp32fc SampleType;
    enum { is_complex   = false         };
    enum { is_integral  = false         };
    enum { size = sizeof(SampleType)    };
};

template<>
struct SampleTraits<kSample64fc>
{
    typedef Ipp32fc SampleType;
    enum { is_complex   = true          };
    enum { is_integral  = false         };
    enum { size = sizeof(SampleType)    };
};

template<>
struct SampleTraits<kSample32sc>
{
    typedef Ipp32sc SampleType;

    enum { is_complex   = true          };
    enum { is_integral  = true          };
    enum { NextWideType = kSample32fc   };
    enum { size = sizeof(SampleType)    };
};

inline size_t SampleSizeByType(SampleType st)
{
    switch(st)
    {
    case kSample12s:     return  SampleTraits<kSample12s> ::size;
    case kSample12sc:    return  SampleTraits<kSample12sc>::size;
    case kSample16s:     return  SampleTraits<kSample16s> ::size;
    case kSample16sc:    return  SampleTraits<kSample16sc>::size;
    case kSample32f:     return  SampleTraits<kSample32f> ::size;
    case kSample32fc:    return  SampleTraits<kSample32fc>::size;
    case kSample64f:     return  SampleTraits<kSample64f> ::size;
    case kSample64fc:    return  SampleTraits<kSample64fc>::size;
    case kSample32sc:    return  SampleTraits<kSample32sc>::size;

    default:        assert(false);
                    return 0;
    }
}

inline bool IsComplexSampleType(SampleType st)
{
    switch(st)
    {
    case kSample12s:     return  SampleTraits<kSample12s> ::is_complex;
    case kSample12sc:    return  SampleTraits<kSample12sc>::is_complex;
    case kSample16s:     return  SampleTraits<kSample16s> ::is_complex;
    case kSample16sc:    return  SampleTraits<kSample16sc>::is_complex;
    case kSample32f:     return  SampleTraits<kSample32f> ::is_complex;
    case kSample32fc:    return  SampleTraits<kSample32fc>::is_complex;
    case kSample64f:     return  SampleTraits<kSample64f> ::is_complex;
    case kSample64fc:    return  SampleTraits<kSample64fc>::is_complex;
    case kSample32sc:    return  SampleTraits<kSample32sc>::is_complex;

    default:        assert(false);
                    return 0;
    }
}

}   //  dsp

#endif