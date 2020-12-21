#include "../src/proto.hh"
#include "gtest/gtest.h"

TEST(proto, breakpoint_request) { // NOLINT
    auto req = R"(
{
    "filename": "/tmp/abc",
    "line_num": 123,
    "column_num": 42,
    "condition": "a"
}
)";
    hgdb::BreakpointRequest r;
    r.parse_payload(req);
    EXPECT_EQ(r.status(), hgdb::status_code::success);
    auto const &bp = r.breakpoint();
    EXPECT_TRUE(bp);
    EXPECT_EQ(bp->filename, "/tmp/abc");
    EXPECT_EQ(bp->line_num, 123);
    EXPECT_EQ(bp->column_num, 42);
    EXPECT_EQ(bp->condition, "a");
}

TEST(proto, breakpoint_request_malformed) {  // NOLINT
    auto req1 = R"(
{
    "line_num": 123,
    "column_num": 42,
    "condition": "a"
}
)";

    auto req2 = R"(
{
    "filename": "/tmp/abc",
    "line_num": "123"
}
)";
    hgdb::BreakpointRequest r;
    r.parse_payload(req1);
    EXPECT_EQ(r.status(), hgdb::status_code::error);
    r = {};
    r.parse_payload(req2);
    EXPECT_EQ(r.status(), hgdb::status_code::error);
    EXPECT_FALSE(r.error_reason().empty());
}

TEST(proto, request_parse_breakpoint) { // NOLINT
    auto req = R"(
{
    "request": true,
    "type": "breakpoint",
    "payload": {
        "filename": "/tmp/abc",
        "line_num": 123
    }
}
)";
    auto r = hgdb::Request::parse_request(req);
    EXPECT_EQ(r->status(), hgdb::status_code::success);
    auto br = dynamic_cast<hgdb::BreakpointRequest*>(r.get());
    EXPECT_NE(br, nullptr);
    EXPECT_EQ(br->breakpoint()->filename, "/tmp/abc");
}