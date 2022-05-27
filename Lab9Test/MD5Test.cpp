#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Lab9/MD5.h"

TEST_CASE("ÑCircle constructor should be work correctló.")
{
	REQUIRE(CMD5::Hashed("md5") == "1BC29B36F623BA82AAF6724FD3B16718");
	REQUIRE(CMD5::Hashed("md4") == "C93D3BF7A7C4AFE94B64E30C2CE39F4F");
	REQUIRE(CMD5::Hashed("") ==    "D41D8CD98F00B204E9800998ECF8427E");
	REQUIRE(CMD5::Hashed("a") ==   "0CC175B9C0F1B6A831C399E269772661");
	REQUIRE(CMD5::Hashed("abc") == "900150983CD24FB0D6963F7D28E17F72");
	REQUIRE(CMD5::Hashed("The quick brown fox jumps over the lazy dog") == "9E107D9D372BB6826BD81D3542A419D6");
	REQUIRE(CMD5::Hashed("The quick brown fox jumps over the lazy cog") == "1055D3E698D289F2AF8663725127BD4B");
}