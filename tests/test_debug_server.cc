#include "../src/debug.hh"
#include "schema.hh"
#include "util.hh"


/*
 * module top;
 *
 * logic a, clk, rst, b;
 *
 * mod dut (.*);
 *
 * endmodule
 *
 * module mod (input  logic a,
 *             input  logic clk,
 *             input  logic rst,
 *             output logic b);
 * // notice the SSA transform here
 * // if (a)
 * //    c = ~a;
 * // else
 * //    c = 1;
 * // c = a;
 * logic c, c_0, c_1, c_2, c_3,;
 * logic t;
 * assign t = a;
 * assign c_0 = ~a; // a = a  en: t
 * assign c_1 = 1;  // a = a  en: ~t
 * assign c_2 = t? c_0: c_1; // a = a en: 1
 * assign c_3 = a; // a = a c = c_2 en: 1
 *
 *
 * always_ff @(posedge clk, posedge rst) begin
 *     if (rst)
 *         b <= 0;    // rst = rst en: rst
 *     else
 *         b <= c;    // rst = rst en: ~rst
 * end
 * endmodule
 *
 */

auto setup_db() {
    // create a mock design, see comments above
    auto db_filename = ":memory:";
    auto db = std::make_unique<hgdb::DebugDatabase>(hgdb::init_debug_db(db_filename));
    db->sync_schema();
    // store

    auto db_client = std::make_unique<hgdb::DebugDatabaseClient>(std::move(db));
    return db_client;
}

auto set_mock() {
    auto vpi = std::unique_ptr<MockVPIProvider>();

    return vpi;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; i++) {
        args.emplace_back(argv[i]);
    }
    auto db = setup_db();

    auto vpi = set_mock();

    auto debug = hgdb::Debugger(std::move(vpi));
    debug.initialize_db(std::move(db));


    return EXIT_SUCCESS;
}