#include "engine/engine.hpp"
#include "gui/layout.hpp"

#include <array>
#include <concepts>
#include <format>
#include <gtest/gtest.h>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>

namespace
{
constexpr LayoutStyle STYLE{
    .divider_width = 20,
    .header_height_pct = 10.0,
    .reset_button_cell_span = 1.5,
};

struct NamedRect
{
  std::string_view name;
  SDL_Rect rect;
};

std::string to_string(const SDL_Rect &r) { return std::format("{{x={}, y={}, w={}, h={}}}", r.x, r.y, r.w, r.h); }

std::string to_string(const NamedRect &r) { return std::format("{} {}", r.name, to_string(r.rect)); }

void _for_each_layout(const std::invocable<const Layout &> auto &check)
{
  for (int w = 400; w <= 1200; w += 101)
  {
    for (int h = 400; h <= 1000; h += 67)
    {
      for (int rows : {1, 2, 9, 10, 16, 30})
      {
        for (int cols : {1, 2, 9, 10, 16, 30})
        {
          SCOPED_TRACE(std::format("w{} h{} r{} c{}", w, h, rows, cols));
          check(Layout{{.width = w, .height = h}, {.rows = rows, .cols = cols}, STYLE});
          if (testing::Test::HasFailure())
          {
            return; // one failing size is enough; avoids hundreds of duplicate failures
          }
        }
      }
    }
  }
}
} // namespace

// input validation

TEST(LayoutTest, NonPositiveGridSizeThrows)
{
  EXPECT_THROW((Layout{{.width = 500, .height = 500}, {.rows = 0, .cols = 10}, STYLE}), std::invalid_argument);
  EXPECT_THROW((Layout{{.width = 500, .height = 500}, {.rows = -1, .cols = 10}, STYLE}), std::invalid_argument);
  EXPECT_THROW((Layout{{.width = 500, .height = 500}, {.rows = 10, .cols = 0}, STYLE}), std::invalid_argument);
  EXPECT_THROW((Layout{{.width = 500, .height = 500}, {.rows = 10, .cols = -1}, STYLE}), std::invalid_argument);
}

TEST(LayoutTest, RectsTooSmallThrows)
{
  EXPECT_THROW((Layout{{.width = 0, .height = 500}, {.rows = 10, .cols = 10}, STYLE}), std::invalid_argument);
  EXPECT_THROW((Layout{{.width = 500, .height = 0}, {.rows = 10, .cols = 10}, STYLE}), std::invalid_argument);
}

// rect sizing and position

TEST(LayoutTest, HeaderIsCenteredHorizontally)
{
  _for_each_layout(
      [](const Layout &layout)
      {
        const int left = layout.header_rect.x - layout.window_rect.x;
        const int right = (layout.window_rect.x + layout.window_rect.w) - (layout.header_rect.x + layout.header_rect.w);

        EXPECT_EQ(left, right);
        EXPECT_EQ(left, STYLE.divider_width);
        EXPECT_EQ(right, STYLE.divider_width);
        EXPECT_EQ(left + layout.header_rect.w + right, layout.window_rect.w);
      });
}

TEST(LayoutTest, BodyIsCenteredHorizontally)
{
  _for_each_layout(
      [](const Layout &layout)
      {
        const int left = layout.body_rect.x - layout.window_rect.x;
        const int right = (layout.window_rect.x + layout.window_rect.w) - (layout.body_rect.x + layout.body_rect.w);

        EXPECT_EQ(left, right);
        EXPECT_EQ(left, STYLE.divider_width);
        EXPECT_EQ(right, STYLE.divider_width);
        EXPECT_EQ(left + layout.body_rect.w + right, layout.window_rect.w);
      });
}

TEST(LayoutTest, GridIsCenteredInGameRect)
{
  _for_each_layout(
      [](const Layout &layout)
      {
        const int left = layout.grid_rect.x - layout.body_rect.x;
        const int right = (layout.body_rect.x + layout.body_rect.w) - (layout.grid_rect.x + layout.grid_rect.w);

        ASSERT_LE(left, right);
        EXPECT_LE(right - left, 1);
      });
}

TEST(LayoutTest, RectsAreDisjoint)
{
  _for_each_layout(
      [](const Layout &layout)
      {
        const NamedRect header{.name = "header rect", .rect = layout.header_rect};
        const NamedRect body{.name = "body rect", .rect = layout.body_rect};
        const std::array regions{header, body};
        const std::array dividers{
            NamedRect{.name = "top divider", .rect = layout.divider_rects.top},
            NamedRect{.name = "middle divider", .rect = layout.divider_rects.middle},
            NamedRect{.name = "bottom divider", .rect = layout.divider_rects.bottom},
            NamedRect{.name = "left divider", .rect = layout.divider_rects.left},
            NamedRect{.name = "right divider", .rect = layout.divider_rects.right},
        };

        // dividers do not intersect with other rects
        for (const auto &[a, b] : std::views::cartesian_product(regions, dividers))
        {
          EXPECT_FALSE(SDL_HasRectIntersection(&a.rect, &b.rect)) << to_string(a) << " overlaps " << to_string(b);
        }

        // header and game rect do not overlap
        EXPECT_FALSE(SDL_HasRectIntersection(&header.rect, &body.rect))
            << to_string(header) << " overlaps " << to_string(body);
      });
}

// cell location

TEST(LayoutTest, LocateCellsAtGridCorners)
{
  const int rows = 8;
  const int cols = 10;
  const Layout layout{{.width = 500, .height = 500}, {.rows = rows, .cols = cols}, STYLE};

  const SDL_Rect &g = layout.grid_rect;
  const int left = g.x;
  const int top = g.y;
  const int right = g.x + g.w - 1;
  const int bottom = g.y + g.h - 1;

  // top left
  EXPECT_EQ(layout.cell_at({.x = left, .y = top}), (CellPosition{.row = 0, .col = 0}));
  EXPECT_EQ(layout.cell_at({.x = left - 1, .y = top}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = left, .y = top - 1}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = left - 1, .y = top - 1}), std::nullopt);

  // top right
  EXPECT_EQ(layout.cell_at({.x = right, .y = top}), (CellPosition{.row = 0, .col = cols - 1}));
  EXPECT_EQ(layout.cell_at({.x = right + 1, .y = top}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = right, .y = top - 1}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = right + 1, .y = top - 1}), std::nullopt);

  // bottom left
  EXPECT_EQ(layout.cell_at({.x = left, .y = bottom}), (CellPosition{.row = rows - 1, .col = 0}));
  EXPECT_EQ(layout.cell_at({.x = left - 1, .y = bottom}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = left, .y = bottom + 1}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = left - 1, .y = bottom + 1}), std::nullopt);

  // bottom right
  EXPECT_EQ(layout.cell_at({.x = right, .y = bottom}), (CellPosition{.row = rows - 1, .col = cols - 1}));
  EXPECT_EQ(layout.cell_at({.x = right + 1, .y = bottom}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = right, .y = bottom + 1}), std::nullopt);
  EXPECT_EQ(layout.cell_at({.x = right + 1, .y = bottom + 1}), std::nullopt);
}

TEST(LayoutTest, LocateCellForEveryPixel)
{
  const int rows = 8;
  const int cols = 10;
  const Layout layout{{.width = 500, .height = 500}, {.rows = rows, .cols = cols}, STYLE};

  const SDL_Rect &g = layout.grid_rect;
  const int cw = layout.cell_width;
  for (const auto [r, c] : std::views::cartesian_product(std::views::iota(0, rows), std::views::iota(0, cols)))
  {
    SCOPED_TRACE(std::format("cell r{} c{}", r, c));
    const int left = g.x + (c * cw);
    const int right = left + cw;
    const int top = g.y + (r * cw);
    const int bottom = top + cw;
    const CellPosition expected{.row = r, .col = c};

    for (const auto [x, y] :
         std::views::cartesian_product(std::views::iota(left, right), std::views::iota(top, bottom)))
    {
      EXPECT_EQ(layout.cell_at({.x = x, .y = y}), expected);
    }
  }
}

// UI element location

class HitTestTest : public testing::Test
{
protected:
  Layout layout{{.width = 500, .height = 600}, {.rows = 8, .cols = 10}, STYLE};
};

TEST_F(HitTestTest, ResetButtonHit)
{
  const SDL_Rect &b = layout.reset_button_rect;
  const int left = b.x;
  const int top = b.y;
  const int right = b.x + b.w;
  const int bottom = b.y + b.h;
  for (const auto [x, y] : std::views::cartesian_product(std::views::iota(left, right), std::views::iota(top, bottom)))
  {
    EXPECT_EQ(layout.hit_test({.x = x, .y = y}), HitTarget{ResetButton{}});
  }
}

TEST_F(HitTestTest, JustOutsideResetButtonMisses)
{
  const SDL_Rect &b = layout.reset_button_rect;
  const int left = b.x;
  const int top = b.y;
  const int right = b.x + b.w - 1;
  const int bottom = b.y + b.h - 1;

  // top left
  EXPECT_EQ(layout.hit_test({.x = left - 1, .y = top}), std::nullopt);
  EXPECT_EQ(layout.hit_test({.x = left, .y = top - 1}), std::nullopt);

  // top right
  EXPECT_EQ(layout.hit_test({.x = right + 1, .y = top}), std::nullopt);
  EXPECT_EQ(layout.hit_test({.x = right, .y = top - 1}), std::nullopt);

  // bottom left
  EXPECT_EQ(layout.hit_test({.x = left - 1, .y = bottom}), std::nullopt);
  EXPECT_EQ(layout.hit_test({.x = left, .y = bottom + 1}), std::nullopt);

  // bottom right
  EXPECT_EQ(layout.hit_test({.x = right + 1, .y = bottom}), std::nullopt);
  EXPECT_EQ(layout.hit_test({.x = right, .y = bottom + 1}), std::nullopt);
}
