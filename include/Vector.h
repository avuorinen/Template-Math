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

#ifndef _TM_VECTOR_H
#define _TM_VECTOR_H

#include "Config.h"

#include "Operators.h"
#include "Functions.h"

#include "Vector4.h"

// std::cout support.
#include <ostream>

#ifndef AOHATAN_MATH_VECTOR11
#define AOHATAN_MATH_VECTOR11 0
#endif


TM_BEGIN_NAMESPACE

namespace Math
{
	namespace Template
	{
		template <unsigned char VectorSize, typename Type>
		struct VectorData
		{
			Type elements[VectorSize];
		};

		// Vector Bases
		template <unsigned char VectorSize, typename Type>
		struct VectorBase;

		template <typename Type>
		struct Vector2Base;

		template <typename Type>
		struct Vector3Base;

		template <typename Type>
		struct Vector4Base;

		// Vector

		template <unsigned char VectorSize, typename Type, class VectorBase =
			typename Operator::CheckType< VectorSize == 2, Operator::TypeResult<Vector2Base<Type> >,
			typename Operator::CheckType< VectorSize == 3, Operator::TypeResult<Vector3Base<Type> >,
			typename Operator::CheckType< VectorSize == 4, Operator::TypeResult<Vector4Base<Type> >,
			typename Operator::TypeResult<VectorBase<VectorSize, Type> > > > >::Result>
		class Vector : public VectorBase
		{

		public:

			// Typedefs //

			typedef unsigned char Index;
			typedef struct VectorData<VectorSize, Type> VectorData;

			// Static //

			static const unsigned short size = VectorSize;

			inline static Vector Zero()
			{
				return VectorBase();
			}


			inline static Vector Create(const Type(&data)[VectorSize])
			{
				VectorData vector;

				Operator::ArrayOperator<VectorSize, Type>::Set(vector.elements, data);
				return Vector(vector);
			}

			template<unsigned char S, typename T, class B>
			inline static Type Dot(const Vector& vector1, const Vector<S, T, B>& vector2)
			{
				return Operator::ArrayOperator< Operator::Size< size, Vector<S, T, B>::size>::result, Type, T>::Dot(vector1.Data(), vector2.Data());			}

			// Data //

			inline Type* Data()
			{
				return this->vectorData.elements;
			}

			inline const Type* Data() const
			{
				return this->vectorData.elements;
			}

			// Get //

			inline Type& operator[](Index index)
			{
				return this->vectorData.elements[index];
			}

			inline Type& Get(Index index)
			{
				if (index < size)
				{
					return this->vectorData.elements[index];
				}

				return 0;
			}

			// Get Const //

			inline const Type operator[](Index index) const
			{
				return this->vectorData.elements[index];
			}

			inline Type Get(Index index) const
			{
				if (index < size)
				{
					return this->vectorData.elements[index];
				}

				return 0;
			}

			// Set //

			template <unsigned char S, typename T, class B>
			inline void Set(const Vector<S, T, B>& vector)
			{
				Operator::ArrayOperator< Operator::Size<size, Vector<S, T, B>::size>::result, Type, T>::Set(Data(), vector.Data());
			}

			// Methods //

			inline Type SqrtMagnitude() const
			{
				return Operator::ArrayOperator<size, Type>::SqrtMagnitude(Data());
			}

			inline Type Magnitude() const
			{
				return Sqrt<Type>(SqrtMagnitude());
			}

			inline Vector Normalize() const
			{
				VectorData data = this->vectorData;
				Operator::ArrayOperator<size, Type>::Normalize(data.elements, Magnitude());

				return Vector(data);
			}

			/// <summary>
			/// Normalize this vector instead of copy.
			/// Returns reference of it self, allows method stacking.
			/// </summary>
			inline Vector& NormalizeThis()
			{
				Operator::ArrayOperator<size, Type>::Normalize(Data(), Magnitude());
				return *this;
			}

			template <unsigned char S, typename T, class V>
			inline Type Dot(const Vector<S, T, V>& vector) const
			{
				return Vector::Dot(*this, vector);
			}

			// Constructors
			Vector() : VectorBase() {}
			Vector(const Type(&data)[size]) : VectorBase() { Operator::ArrayOperator<size, Type>::Set(Data(), data); }

			Vector(const VectorData data) : VectorBase(data) {}
			Vector(const Vector& vector) : VectorBase(vector.vectorData) {}

			#if AOHATAN_MATH_VECTOR11

			// C++ 11 Magic //
			template<typename... Args>
			/// <summary>
			/// This constuctor accept unlimited parameters...
			/// BUT in reality it only accept as much as vector size.
			/// This will throw compiler error but not syntax!
			/// </summary>
			Vector(Args... value) : VectorBase({ { static_cast<Type>(value)... } }) { }

			#else

			// Common Constructors

			// Reference values makes void* throw syntax error instead of run time failure.
			typedef const typename Operator::EnableIf<2 == VectorSize, Type>::type& Vector2Type;
			typedef const typename Operator::EnableIf<3 == VectorSize, Type>::type& Vector3Type;
			typedef const typename Operator::EnableIf<4 == VectorSize, Type>::type& Vector4Type;

			Vector(Vector2Type x, Vector2Type y) : VectorBase(Vector::VectorData{ x, y }) {}
			Vector(Vector3Type x, Vector3Type y, Vector3Type z) : VectorBase(Vector::VectorData{ x, y, z }) {}
			Vector(Vector4Type x, Vector4Type y, Vector4Type z, Vector4Type w) : VectorBase(Vector::VectorData{ x, y, z, w }) {}

			#endif /* AOHATAN_MATH_VECTOR11 */

			// Same Type Assingment
			inline Vector& operator=(const Vector& vector)
			{
				if (this == &vector)
				{
					return *this;
				}

				this->vectorData = vector.vectorData;

				return *this;

			}

			// Generic Type Assingment
			template<unsigned char S, typename T, class B>
			inline Vector& operator=(const Vector<S, T, B>& vector)
			{
				Operator::ArrayOperator< Operator::Size<size, Vector<S, T, B>::size>::result, Type, T>::Set(Data(), vector.Data());
				return *this;
			}

			// Operators //

			template<unsigned char S, typename T, class B>
			inline operator Vector<S, T, B>()
			{
				Vector<S, T, B> vector;
				//vector.Set<size, Type, VectorBase>(*this);
				Operator::ArrayOperator< Operator::Size<size, Vector<S, T, B>::size>::result, Type, T>::Set(vector.Data(), Data());

				return vector;
			}

			// Comparision //

			template <unsigned char S, typename T, class V>
			inline bool operator==(const Vector<S, T, V>& vector) const
			{
				return Operator::ArrayOperator< Operator::Size<size, Vector<S, T, V>::size>::result, Type, T>::Equals(Data(), vector.Data());
			}

			template <unsigned char S, typename T, class V>
			inline bool operator!=(const Vector<S, T, V>& vector) const
			{
				return !operator==(vector);
			}


			// Unary operators //

			inline Vector operator-() const
			{
				VectorData data = this->vectorData;
				Operator::ArrayOperator<size, Type>::Mul(data.elements, -1);
				return Vector(data);
			}

			// Operators +-*/ //
			// Templates, allows inheritanced classes use operators.

			template <unsigned char S, typename T, class V>
			inline Vector operator+(const Vector<S, T, V>& vector) const
			{
				VectorData data = this->vectorData;
				Operator::ArrayOperator< Operator::Size<size, Vector<S, T, V>::size>::result, Type, T>::Add(data.elements, vector.Data());

				return Vector(data);
			}

			template <unsigned char S, typename T, class V>
			inline Vector& operator+=(const Vector<S, T, V>& vector)
			{
				Operator::ArrayOperator< Operator::Size<size, Vector<S, T, V>::size>::result, Type, T>::Add(Data(), vector.Data());
				return *this;
			}

			template <unsigned char S, typename T, class V>
			inline Vector operator-(const Vector<S, T, V>& vector) const
			{
				VectorData data = this->vectorData;
				Operator::ArrayOperator< Operator::Size<size, Vector<S, T, V>::size>::result, Type, T>::Sub(data.elements, vector.Data());

				return Vector(data);
			}

			template <unsigned char S, typename T, class V>
			inline Vector& operator-=(const Vector<S, T, V>& vector)
			{
				Operator::ArrayOperator< Operator::Size<size, Vector<S, T, V>::size>::result, Type, T>::Sub(Data(), vector.Data());
				return *this;
			}

			inline Vector operator*(const Type scalar) const
			{
				VectorData data = this->vectorData;
				Operator::ArrayOperator< size, Type>::Mul(data.elements, scalar);

				return Vector(data);
			}

			inline Vector operator*=(const Type scalar)
			{
				Operator::ArrayOperator< size, Type>::Mul(Data(), scalar);
				return *this;
			}

			inline Vector operator/(const Type scalar) const
			{
				VectorData data = this->vectorData;
				Operator::ArrayOperator< size, Type>::Div(data.elements, scalar);

				return Vector(data);
			}

			inline Vector operator/=(const Type scalar)
			{
				Operator::ArrayOperator< size, Type>::Div(Data(), scalar);
				return *this;
			}

			template <unsigned char S, typename T, class V>
			inline Type operator*(const Vector<S, T, V>& vector)
			{
				return Dot(vector);
			}

			// Friend operators //

			template<unsigned char S, typename T, class B>
			friend inline Vector<S, T, B> operator*(const Type scalar, const Vector<S, T, B>& vector);

			template<unsigned char S, typename T, class B>
			friend inline std::ostream& operator<<(std::ostream &out, const Vector<S, T, B>& vector);
		};

		template<unsigned char S, typename T, class B>
		inline std::ostream& operator<<(std::ostream &out, const Vector<S, T, B>& vector)
		{
			out << vector.size << "[ ";

			for (unsigned char i = 0; i != vector.size; i++)
			{
				out << vector[i] << " ";
			}

			out << "]";

			return out;
		}

		template<unsigned char S, typename T, class B>
		inline Vector<S, T, B> operator*(const T scalar, const Vector<S, T, B>& vector)
		{
			return vector * scalar;
		}

		// Vector Bases
		// TODO: Component initialization?

		template <unsigned char VectorSize, typename Type>
		struct VectorBase
		{
			VectorData<VectorSize, Type> vectorData;

			VectorBase() {}
			VectorBase(VectorData<VectorSize, Type> data) : vectorData(data) {}
		};

		template <typename Type>
		struct Vector3Base
		{
			typedef Vector<3, Type, Vector3Base > RealVector;

			union
			{
				VectorData<3, Type> vectorData;
				struct
				{
					Type x, y, z;
				};
			};

			Vector3Base() {}
			Vector3Base(VectorData<3, Type> data) : vectorData(data) {}

			static void Cross(Vector<3, Type, Vector3Base >& vector, const Vector<3, Type, Vector3Base >& rhs)
			{
				Type x(vector.y * rhs.z - vector.z * rhs.y), y(vector.z * rhs.x - vector.x * rhs.z), z(vector.x * rhs.y - vector.y * rhs.x);

				vector.x = x;
				vector.y = y;
				vector.z = z;
			}

			inline RealVector Cross(const RealVector& rhs) const
			{
				RealVector vector = RealVector({ { x, y, z } });

				Vector3Base::Cross(vector, rhs);
				return vector;
			}

			inline RealVector& CrossThis(const RealVector& rhs)
			{
				Vector3Base::Cross(*this, rhs);
				return *this;
			}
		};

		template <typename Type>
		struct Vector2Base
		{
			typedef Vector<2, Type, Vector2Base > RealVector;

			union
			{
				VectorData<2, Type> vectorData;
				struct
				{
					Type x, y;
				};
			};

			Vector2Base() {}
			Vector2Base(VectorData<2, Type> data) : vectorData(data) {}
			
			static Type Cross(RealVector& vector, const RealVector& rhs)
			{
				return vector.x * rhs.y - vector.y * rhs.x;
			}

			inline Type Cross(const RealVector& rhs) const
			{
				RealVector vector = RealVector({ x, y });

				return Vector2Base::Cross(vector, rhs);
			}

		};

		template <typename Type>
		struct Vector4Base
		{
			union
			{
				VectorData<4, Type> vectorData;
				struct
				{
					Type x, y, z, w;
				};
			};

			Vector4Base() {}
			Vector4Base(VectorData<4, Type> data) : vectorData(data) {}

		};

	}

	using Template::Vector;

	typedef Template::Vector<2, float, Template::Vector2Base<float> > Vector2;
	typedef Template::Vector<3, float, Template::Vector3Base<float> > Vector3;
	typedef Template::Vector<4, float, Template::Vector4Base<float> > Vector4;

}

TM_END_NAMESPACE

#endif /* _TM_VECTOR_H */
