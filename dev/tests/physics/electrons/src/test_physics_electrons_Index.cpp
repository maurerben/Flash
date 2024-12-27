#define CATCH_PHYSICS_MAIN
#include <physics/electrons/Index.h>
#include <catch2/catch_all.hpp>
#include <Eigen/Dense>
#include <cstdint>
#include <iostream>

using namespace flash::physics::electrons;

TEST_CASE("Test flash::physics::electrons::Index class") {

  Index index(5, 1, 2);
  Index jndex(5, 1, 2);
  Index kndex(4, 1, 1);
  Index lndex(2, 0, 2);

  SECTION("Test constructor") {
    REQUIRE(index.getElectron() == 5);
    REQUIRE(index.getKpoint() == 1);
    REQUIRE(index.getBand() == 2);
  }

  SECTION("Test operator==") {
    REQUIRE(index == jndex);
    REQUIRE_FALSE(index == kndex);
  }

  SECTION("Test operator!=") {
    REQUIRE_FALSE(index != jndex);
    REQUIRE(index != kndex);
  }

  SECTION("Test isOccupied") {
    Eigen::Vector<std::double_t, 6> occupations;
    occupations << 1.0, 0.0, 0.01, 2.0, 0.0, 2.0;
    REQUIRE(index.isOccupied(occupations, 0.0));
    REQUIRE_FALSE(kndex.isOccupied(occupations, 0.0));
  }

  SECTION("Test isUnoccupied") {
    Eigen::Vector<std::double_t, 6> occupations;
    occupations << 1.0, 0.0, 0.01, 2.0, 0.0, 2.0;
    REQUIRE_FALSE(index.isUnoccupied(occupations, 0.0));
    REQUIRE(kndex.isUnoccupied(occupations, 0.0));
  }

}

TEST_CASE("Test flash::physics::electrons::Indices class") {

    // Create indices for and 2 k-points 3 bands
    Indices indices(2, 3);

    SECTION("Test constructor") {
        REQUIRE(indices.size() == 6);

        REQUIRE(indices[0] == Index(0, 0, 0));
        REQUIRE(indices[1] == Index(1, 0, 1));
        REQUIRE(indices[2] == Index(2, 0, 2));
        REQUIRE(indices[3] == Index(3, 1, 0));
        REQUIRE(indices[4] == Index(4, 1, 1));
        REQUIRE(indices[5] == Index(5, 1, 2));
    }

    SECTION("Test size") {
        REQUIRE(indices.size() == 6);
    }

    SECTION("Test getElectrons") {
        Eigen::Vector<Eigen::Index, Eigen::Dynamic> electrons = indices.getElectrons();
        REQUIRE(electrons[0] == 0);
        REQUIRE(electrons[1] == 1);
        REQUIRE(electrons[2] == 2);
        REQUIRE(electrons[3] == 3);
        REQUIRE(electrons[4] == 4);
        REQUIRE(electrons[5] == 5);
    }

    SECTION("Test getKpoints") {
        Eigen::Vector<Eigen::Index, Eigen::Dynamic> kpoints = indices.getKpoints();
        REQUIRE(kpoints[0] == 0);
        REQUIRE(kpoints[1] == 0);
        REQUIRE(kpoints[2] == 0);
        REQUIRE(kpoints[3] == 1);
        REQUIRE(kpoints[4] == 1);
        REQUIRE(kpoints[5] == 1);
    }

    SECTION("Test getBands") {
        Eigen::Vector<Eigen::Index, Eigen::Dynamic> bands = indices.getBands();
        REQUIRE(bands[0] == 0);
        REQUIRE(bands[1] == 1);
        REQUIRE(bands[2] == 2);
        REQUIRE(bands[3] == 0);
        REQUIRE(bands[4] == 1);
        REQUIRE(bands[5] == 2);
    }

    SECTION("Test atKpoint") {
      auto indicesAtKpoint1 = indices.atKpoint(1);
      REQUIRE(indicesAtKpoint1[0] == indices[3]);
      REQUIRE(indicesAtKpoint1[1] == indices[4]);
      REQUIRE(indicesAtKpoint1[2] == indices[5]);
      REQUIRE_THROWS(indices.atKpoint(-1));
      REQUIRE_THROWS(indices.atKpoint(2)); // Out of bounds
    }

    SECTION("Test atBand") {
      auto indicesAtBand2 = indices.atBand(2);
      REQUIRE(indicesAtBand2[0] == indices[2]);
      REQUIRE(indicesAtBand2[1] == indices[5]);
      REQUIRE_THROWS(indices.atBand(-1));
      REQUIRE_THROWS(indices.atBand(3)); // Out of bounds
    }

    SECTION("Test atKpointBand") {
      REQUIRE(indices.atKpointBand(0, 0) == indices[0]);
      REQUIRE(indices.atKpointBand(1, 2) == indices[5]);
      REQUIRE_THROWS(indices.atKpointBand(2, 0)); // Out of bounds
      REQUIRE_THROWS(indices.atKpointBand(0, 3)); // Out of bounds
    }

    SECTION("Test getElectron") {
        REQUIRE(indices.atElectron(0) == indices[0]);
        REQUIRE(indices.atElectron(5) == indices[5]);
        REQUIRE_THROWS(indices.atElectron(6)); // Out of bounds
    }

    SECTION("Test separateByOccupation") {
      Eigen::Vector<std::double_t, 6> occupations;
      occupations << 1.0, 0.0, 0.01,
                     2.0, 0.0, 0.0;
      auto [occupiedIndices, unoccupiedIndices]
          = indices.separateByOccupation(occupations, 0.001);
      REQUIRE(occupiedIndices[0] == indices[0]);
      REQUIRE(occupiedIndices[1] == indices[2]);
      REQUIRE(occupiedIndices[2] == indices[3]);
      REQUIRE(unoccupiedIndices[0] == indices[1]);
      REQUIRE(unoccupiedIndices[1] == indices[4]);
      REQUIRE(unoccupiedIndices[2] == indices[5]);

      // No occupied indices
      occupations << 0.0, 0.0, 0.0,
                     0.0, 0.0, 0.0;
      REQUIRE_THROWS(indices.separateByOccupation(occupations, 0.001));

      // No unoccupied indices
      occupations << 1.0, 1.0, 1.0,
                     1.0, 1.0, 1.0;
      REQUIRE_THROWS(indices.separateByOccupation(occupations, 0.001));
    }

    SECTION("Test Iterator") {
        int idx = 0;
        for (auto it = indices.begin(); it != indices.end(); ++it) {
            REQUIRE(*it == indices[idx]);
            ++idx;
        }

        idx = 0;
        for (auto index : indices) {
            REQUIRE(index == indices[idx]);
            ++idx;
        }

    }

}