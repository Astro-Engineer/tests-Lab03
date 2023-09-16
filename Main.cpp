#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "catch_reporter_github.hpp"
// Some Windows BS I guess included by Catch?
#ifdef far
#undef far
#endif
#ifdef near
#undef near
#endif
#include "Actor.h"
#include "CollisionComponent.h"

// This is janky but doing it this way to account for weird include
// dependencies people may have introduced into CollisionComponent.cpp
#include <algorithm>
#include <vector>
// Create dummy implementations for a few SDL functions/macros
#ifdef SDL_assert
#undef SDL_assert
#endif
#define SDL_assert(condition) REQUIRE(condition)

#ifdef SDL_assert_release
#undef SDL_assert_release
#endif
#define SDL_assert_release(condition) REQUIRE(condition)

// Skip over SDL logs completely
static void SDL_Log(...)
{
}

#include "CollisionComponent.cpp"

TEST_CASE("GetMin()")
{
	SECTION("GetMin() - Square dimensions, scale 1")
	{
		Actor a(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c(&a);
		c.SetSize(32.0f, 32.0f);
		Vector2 min = c.GetMin();
		REQUIRE(min.x == Approx(50.0f - 16.0f));
		REQUIRE(min.y == Approx(100.0f - 16.0f));
	}
	SECTION("GetMin() - Non-uniform dimensions, scale 1")
	{
		Actor a(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c(&a);
		c.SetSize(32.0f, 64.0f);
		Vector2 min = c.GetMin();
		REQUIRE(min.x == Approx(50.0f - 16.0f));
		REQUIRE(min.y == Approx(100.0f - 32.0f));
	}
	SECTION("GetMin() - Non-uniform dimensions, scale 1")
	{
		Actor a(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c(&a);
		c.SetSize(64.0f, 32.0f);
		Vector2 min = c.GetMin();
		REQUIRE(min.x == Approx(50.0f - 32.0f));
		REQUIRE(min.y == Approx(100.0f - 16.0f));
	}
	SECTION("GetMin() - Square dimensions, scale 13")
	{
		Actor a(Vector2(50.0f, 100.0f), 13.0f);
		CollisionComponent c(&a);
		c.SetSize(32.0f, 32.0f);
		Vector2 min = c.GetMin();
		REQUIRE(min.x == Approx(50.0f - 16.0f * 13.0f));
		REQUIRE(min.y == Approx(100.0f - 16.0f * 13.0f));
	}
	SECTION("GetMin() - Non-uniform dimensions, scale 13")
	{
		Actor a(Vector2(50.0f, 100.0f), 13.0f);
		CollisionComponent c(&a);
		c.SetSize(64.0f, 32.0f);
		Vector2 min = c.GetMin();
		REQUIRE(min.x == Approx(50.0f - 32.0f * 13.0f));
		REQUIRE(min.y == Approx(100.0f - 16.0f * 13.0f));
	}
}

TEST_CASE("GetMax()")
{
	SECTION("GetMax() - Square dimensions, scale 1")
	{
		Actor a(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c(&a);
		c.SetSize(32.0f, 32.0f);
		Vector2 max = c.GetMax();
		REQUIRE(max.x == Approx(50.0f + 16.0f));
		REQUIRE(max.y == Approx(100.0f + 16.0f));
	}
	SECTION("GetMax() - Non-uniform dimensions, scale 1")
	{
		Actor a(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c(&a);
		c.SetSize(32.0f, 64.0f);
		Vector2 max = c.GetMax();
		REQUIRE(max.x == Approx(50.0f + 16.0f));
		REQUIRE(max.y == Approx(100.0f + 32.0f));
	}
	SECTION("GetMax() - Non-uniform dimensions, scale 1")
	{
		Actor a(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c(&a);
		c.SetSize(64.0f, 32.0f);
		Vector2 max = c.GetMax();
		REQUIRE(max.x == Approx(50.0f + 32.0f));
		REQUIRE(max.y == Approx(100.0f + 16.0f));
	}
	SECTION("GetMax() - Square dimensions, scale 13")
	{
		Actor a(Vector2(50.0f, 100.0f), 13.0f);
		CollisionComponent c(&a);
		c.SetSize(32.0f, 32.0f);
		Vector2 max = c.GetMax();
		REQUIRE(max.x == Approx(50.0f + 16.0f * 13.0f));
		REQUIRE(max.y == Approx(100.0f + 16.0f * 13.0f));
	}
	SECTION("GetMax() - Non-uniform dimensions, scale 13")
	{
		Actor a(Vector2(50.0f, 100.0f), 13.0f);
		CollisionComponent c(&a);
		c.SetSize(64.0f, 32.0f);
		Vector2 max = c.GetMax();
		REQUIRE(max.x == Approx(50.0f + 32.0f * 13.0f));
		REQUIRE(max.y == Approx(100.0f + 16.0f * 13.0f));
	}
}

TEST_CASE("Intersect() - They don't intersect")
{
	SECTION("Intersect() - They don't intersect - Square dimensions - Below")
	{
		Actor a1(Vector2(50.0f, 70.0f), 1.0f);
		Actor a2(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
	SECTION("Intersect() - They don't intersect - Square dimensions - Above")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(50.0f, 74.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
	SECTION("Intersect() - They don't intersect - Square dimensions - Right")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(20.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
	SECTION("Intersect() - They don't intersect - Square dimensions - Left")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(100.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
	SECTION("Intersect() - They don't intersect - One is wide - Below")
	{
		Actor a1(Vector2(50.0f, 70.0f), 1.0f);
		Actor a2(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(500.0f, 25.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
	SECTION("Intersect() - They don't intersect - One is wide - Above")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(50.0f, 74.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(500.0f, 25.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
	SECTION("Intersect() - They don't intersect - One is tall - Right")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(20.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 500.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
	SECTION("Intersect() - They don't intersect - One is tall - Left")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(100.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 500.0f);
		REQUIRE(!c1.Intersect(&c2));
	}
}

TEST_CASE("Intersect() - They do intersect")
{
	SECTION("Intersect() - They do intersect - Square dimensions - Below")
	{
		Actor a1(Vector2(50.0f, 70.0f), 1.0f);
		Actor a2(Vector2(50.0f, 80.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(c1.Intersect(&c2));
	}
	SECTION("Intersect() - They do intersect - Square dimensions - Above")
	{
		Actor a1(Vector2(50.0f, 80.0f), 1.0f);
		Actor a2(Vector2(50.0f, 74.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(c1.Intersect(&c2));
	}
	SECTION("Intersect() - They do intersect - Square dimensions - Right")
	{
		Actor a1(Vector2(30.0f, 100.0f), 1.0f);
		Actor a2(Vector2(20.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(c1.Intersect(&c2));
	}
	SECTION("Intersect() - They do intersect - Square dimensions - Left")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(60.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		REQUIRE(c1.Intersect(&c2));
	}
	SECTION("Intersect() - They do intersect - One is wide - Below")
	{
		Actor a1(Vector2(50.0f, 70.0f), 1.0f);
		Actor a2(Vector2(50.0f, 90.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(500.0f, 25.0f);
		REQUIRE(c1.Intersect(&c2));
	}
	SECTION("Intersect() - They do intersect - One is wide - Above")
	{
		Actor a1(Vector2(50.0f, 90.0f), 1.0f);
		Actor a2(Vector2(50.0f, 74.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(500.0f, 25.0f);
		REQUIRE(c1.Intersect(&c2));
	}
	SECTION("Intersect() - They do intersect - One is tall - Right")
	{
		Actor a1(Vector2(40.0f, 100.0f), 1.0f);
		Actor a2(Vector2(20.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 500.0f);
		REQUIRE(c1.Intersect(&c2));
	}
	SECTION("Intersect() - They do intersect - One is tall - Left")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(60.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 500.0f);
		REQUIRE(c1.Intersect(&c2));
	}
}

TEST_CASE("GetMinOverlap() - They don't intersect")
{
	SECTION("GetMinOverlap() - They don't intersect - Square dimensions - Below")
	{
		Actor a1(Vector2(50.0f, 70.0f), 1.0f);
		Actor a2(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
	SECTION("GetMinOverlap() - They don't intersect - Square dimensions - Above")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(50.0f, 74.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
	SECTION("GetMinOverlap() - They don't intersect - Square dimensions - Right")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(20.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
	SECTION("GetMinOverlap() - They don't intersect - Square dimensions - Left")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(100.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 25.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
	SECTION("GetMinOverlap() - They don't intersect - One is wide - Below")
	{
		Actor a1(Vector2(50.0f, 70.0f), 1.0f);
		Actor a2(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(500.0f, 25.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
	SECTION("GetMinOverlap() - They don't intersect - One is wide - Above")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(50.0f, 74.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(500.0f, 25.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
	SECTION("GetMinOverlap() - They don't intersect - One is tall - Right")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(20.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 500.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
	SECTION("GetMinOverlap() - They don't intersect - One is tall - Left")
	{
		Actor a1(Vector2(50.0f, 100.0f), 1.0f);
		Actor a2(Vector2(100.0f, 100.0f), 1.0f);
		CollisionComponent c1(&a1);
		c1.SetSize(25.0f, 25.0f);
		CollisionComponent c2(&a2);
		c2.SetSize(25.0f, 500.0f);
		Vector2 offset;
		REQUIRE(c1.GetMinOverlap(&c2, offset) == CollSide::None);
	}
}

TEST_CASE("GetMinOverlap() - Single overlap side")
{
	SECTION("GetMinOverlap() - Single overlap side - Top")
	{
		Actor thisActor(Vector2(50.0f, 85.0f), 1.0f);
		Actor otherActor(Vector2(50.0f, 110.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(100.0f, 32.0f);
		Vector2 offset;
		REQUIRE(thisCC.GetMinOverlap(&otherCC, offset) == CollSide::Top);
		REQUIRE(offset.x == Approx(0.0f));
		REQUIRE(offset.y == Approx(-7.0f));
	}
	SECTION("GetMinOverlap() - Single overlap side - Bottom")
	{
		Actor thisActor(Vector2(50.0f, 128.0f), 1.0f);
		Actor otherActor(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(100.0f, 32.0f);
		Vector2 offset;
		REQUIRE(thisCC.GetMinOverlap(&otherCC, offset) == CollSide::Bottom);
		REQUIRE(offset.x == Approx(0.0f));
		REQUIRE(offset.y == Approx(4.0f));
	}
	SECTION("GetMinOverlap() - Single overlap side - Left")
	{
		Actor thisActor(Vector2(23.0f, 100.0f), 1.0f);
		Actor otherActor(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(32.0f, 32.0f);
		Vector2 offset;
		REQUIRE(thisCC.GetMinOverlap(&otherCC, offset) == CollSide::Left);
		REQUIRE(offset.x == Approx(-5.0f));
		REQUIRE(offset.y == Approx(0.0f));
	}
	SECTION("GetMinOverlap() - Single overlap side - Right")
	{
		Actor thisActor(Vector2(74.0f, 100.0f), 1.0f);
		Actor otherActor(Vector2(50.0f, 100.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(32.0f, 32.0f);
		Vector2 offset;
		REQUIRE(thisCC.GetMinOverlap(&otherCC, offset) == CollSide::Right);
		REQUIRE(offset.x == Approx(8.0f));
		REQUIRE(offset.y == Approx(0.0f));
	}
}

TEST_CASE("GetMinOverlap() - Tie")
{
	SECTION("GetMinOverlap() - Tie - Top or Left")
	{
		Actor thisActor(Vector2(32.0f, 32.0f), 1.0f);
		Actor otherActor(Vector2(64.0f, 64.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(32.0f, 32.0f);
		Vector2 offset;
		CollSide side = thisCC.GetMinOverlap(&otherCC, offset);
		REQUIRE((side == CollSide::Top || side == CollSide::Left));
		REQUIRE(offset.x == Approx(0.0f));
		REQUIRE(offset.y == Approx(0.0f));
	}
	SECTION("GetMinOverlap() - Tie - Top or Right")
	{
		Actor thisActor(Vector2(96.0f, 32.0f), 1.0f);
		Actor otherActor(Vector2(64.0f, 64.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(32.0f, 32.0f);
		Vector2 offset;
		CollSide side = thisCC.GetMinOverlap(&otherCC, offset);
		REQUIRE((side == CollSide::Top || side == CollSide::Right));
		REQUIRE(offset.x == Approx(0.0f));
		REQUIRE(offset.y == Approx(0.0f));
	}
	SECTION("GetMinOverlap() - Tie - Bottom or Left")
	{
		Actor thisActor(Vector2(32.0f, 96.0f), 1.0f);
		Actor otherActor(Vector2(64.0f, 64.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(32.0f, 32.0f);
		Vector2 offset;
		CollSide side = thisCC.GetMinOverlap(&otherCC, offset);
		REQUIRE((side == CollSide::Bottom || side == CollSide::Left));
		REQUIRE(offset.x == Approx(0.0f));
		REQUIRE(offset.y == Approx(0.0f));
	}
	SECTION("GetMinOverlap() - Tie - Bottom or Right")
	{
		Actor thisActor(Vector2(96.0f, 96.0f), 1.0f);
		Actor otherActor(Vector2(64.0f, 64.0f), 1.0f);
		CollisionComponent thisCC(&thisActor);
		thisCC.SetSize(32.0f, 32.0f);
		CollisionComponent otherCC(&otherActor);
		otherCC.SetSize(32.0f, 32.0f);
		Vector2 offset;
		CollSide side = thisCC.GetMinOverlap(&otherCC, offset);
		REQUIRE((side == CollSide::Bottom || side == CollSide::Right));
		REQUIRE(offset.x == Approx(0.0f));
		REQUIRE(offset.y == Approx(0.0f));
	}
}
