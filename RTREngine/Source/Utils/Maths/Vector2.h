#pragma once

namespace rtr
{
	template <typename T = float>
	struct Vector2
	{
	public:

		T X;
		T Y;

		constexpr Vector2() : X(0), Y(0) {}
		constexpr Vector2(T x, T y) : X(x), Y(y) {}
		constexpr Vector2(const Vector2& other) : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y)) {}
		constexpr Vector2(Vector2&& other) : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y)) {}

		// Assignment and comparison
		constexpr Vector2& operator=(const Vector2& other) { X = other.X; Y = other.Y; return *this; }
		constexpr Vector2& operator=(Vector2&& other) { X = other.X; Y = other.Y; return *this; }
		constexpr Vector2& operator=(T value) { X = value; Y = value; return *this; }
		constexpr bool operator==(Vector2& other) { return (X = other.X) && (Y == other.Y); }

	};
}