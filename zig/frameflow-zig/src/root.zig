//! By convention, root.zig is the root source file when making a package.
const std = @import("std");
const Io = std.Io;

/// This is a documentation comment to explain the `printAnotherMessage` function below.
///
/// Accepting an `Io.Writer` instance is a handy way to write reusable code.
pub fn printAnotherMessage(writer: *Io.Writer) Io.Writer.Error!void {
    try writer.print("Run `zig build test` to run the tests.\n", .{});
}

pub fn add(a: i32, b: i32) i32 {
    return a + b;
}

test "basic add functionality" {
    try std.testing.expect(add(3, 7) == 10);
}

export fn mylib_add(a: c_int, b: c_int) callconv(.c) c_int {
    return a + b;
}

export fn mylib_greet(name: [*:0]const u8) callconv(.c) void {
    std.debug.print("hello, {s}\n", .{std.mem.span(name)});
}
