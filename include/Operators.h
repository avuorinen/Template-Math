/*
Copyright (c) 2016 Atte Vuorinen <attevuorinen@gmail.com>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _TM_OPERATORS_H
#define _TM_OPERATORS_H

#include "Config.h"

TM_BEGIN_NAMESPACE

namespace Math
{
	namespace Operator
	{
		template<class T>
		struct TypeResult
		{
			typedef T Result;
		};

		template<class>
		struct Empty {};

		template<bool, class True, class False>
		struct CheckType
		{
			typedef typename False::Result Result;
		};

		template<class True, class False>
		struct CheckType<true, True, False>
		{
			typedef typename True::Result Result;
		};


		// Helper //

		template<bool, class T = void>
		struct EnableIf
		{
			typedef void* type;
		};

		template <class T>
		struct EnableIf<true, T>
		{
			typedef T type;
		};


		template<unsigned char S1, unsigned char S2>
		struct Size
		{
			static const unsigned char result = S1 > S2 ? S2 : S1;
		};

		/// Array Operators ///

		template <unsigned int Index, typename Type1, typename Type2 = Type1, typename Specialization = Type1>
		struct ArrayOperator
		{
			static inline bool Equals(const Type1* v1, const Type2* v2)
			{
				return *v1 == *v2 && ArrayOperator<Index - 1, Type1, Type2, Specialization>::Equals(v1 + 1, v2 + 1);
			}

			static inline void Set(Type1* v1, const Type2* v2)
			{
				*v1 = *v2;
				ArrayOperator<Index - 1, Type1, Type2, Specialization>::Set(v1 + 1, v2 + 1);
			}

			static inline void Add(Type1* v1, const Type2* v2)
			{
				*v1 += *v2;
				ArrayOperator<Index - 1, Type1, Type2, Specialization>::Add(v1 + 1, v2 + 1);
			}

			static inline void Sub(Type1* v1, const Type2* v2)
			{
				*v1 -= *v2;
				ArrayOperator<Index - 1, Type1, Type2, Specialization>::Sub(v1 + 1, v2 + 1);
			}

			static inline void Mul(Type1* value, const Type1 scalar)
			{
				*value *= scalar;
				ArrayOperator<Index - 1, Type1, Type2, Specialization>::Mul(value + 1, scalar);
			}

			static inline void Div(Type1* value, const Type1 scalar)
			{
				*value /= scalar;
				ArrayOperator<Index - 1, Type1, Specialization>::Div(value + 1, scalar);
			}

			static inline Type1 SqrtMagnitude(const Type1* value)
			{
				return *value * *value + ArrayOperator<Index - 1, Type1, Type2, Specialization>::SqrtMagnitude(value + 1);
			}

			static inline void Normalize(Type1* value, Type1 magnitude)
			{
				*value /= magnitude;
				ArrayOperator<Index - 1, Type1, Type2, Specialization>::Normalize(value + 1, magnitude);
			}

			static inline Type1 Dot(const Type1* v1, const Type2* v2)
			{
				return *v1 * *v2 + ArrayOperator<Index - 1, Type1, Type2, Specialization>::Dot(v1 + 1, v2 + 1);
			}
		};

		template <typename Type1, typename Type2, typename Specialization>
		struct ArrayOperator<1, Type1, Type2, Specialization>
		{
			static inline bool Equals(const Type1* v1, const Type2* v2)
			{
				return *v1 == *v2;
			}

			static inline void Set(Type1* v1, const Type2* v2)
			{
				*v1 = *v2;
			}

			static inline void Add(Type1* v1, const Type2* v2)
			{
				*v1 += *v2;
			}

			static inline void Sub(Type1* v1, const Type2* v2)
			{
				*v1 -= *v2;
			}

			static inline void Mul(Type1* value, const Type1 scalar)
			{
				*value *= scalar;
			}

			static inline void Div(Type1* value, const Type1 scalar)
			{
				*value /= scalar;
			}

			static inline Type1 SqrtMagnitude(const Type1* value)
			{
				return *value * *value;
			}

			static inline void Normalize(Type1* value, Type1 magnitude)
			{
				*value /= magnitude;
			}

			static inline Type1 Dot(const Type1* v1, const Type2* v2)
			{
				return *v1 * *v2;
			}
		};


		// Matrix Transpose Loop UnRoller.
		// Current not used but left for fun.
		// MatrixTransposeOperator<Row, Column, Row, Column, Type>::Transpose(Type* to, Type* from);
		// First Row and Column also known as R and C are used as indices.
		// Second Row and Column also known Row and Column are used to store orginal Nested Array sizes.

		template<unsigned char R, unsigned char C, unsigned char Row, unsigned char Column, typename Type>
		struct MatrixTransposeOperator
		{
			static inline void Transpose(Type (&v1)[Column][Row], const Type (&v2)[Row][Column])
			{
				v1[C-1][R-1] = v2[R-1][C-1];
				MatrixTransposeOperator<R, C - 1, Row, Column, Type>::Transpose(v1, v2);
			}
		};

		template<unsigned char R, unsigned char Row, unsigned char Column, typename Type>
		struct MatrixTransposeOperator<R, 0, Row, Column, Type>
		{
			static inline void Transpose(Type(&v1)[Column][Row], const Type(&v2)[Row][Column])
			{
				v1[0][R-1] = v2[R-1][0];
				MatrixTransposeOperator<R - 1, Column, Row, Column, Type>::Transpose(v1, v2);
			}
		};

		template<unsigned C, unsigned char Row, unsigned char Column, typename Type>
		struct MatrixTransposeOperator<0, C, Row, Column, Type>
		{
			static inline void Transpose(Type(&v1)[Column][Row], const Type(&v2)[Row][Column])
			{
				v1[0][0] = v2[0][0];
			}
		};
	}
}

TM_END_NAMESPACE

#endif /* _TM_OPERATORS_H */
