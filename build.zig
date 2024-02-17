const std = @import("std");
const rl = @import("raylib/src/build.zig");

fn getSrcDir() []const u8 {
    return std.fs.path.dirname(@src().file).?;
}

const src_dir = getSrcDir();

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});

    const optimize = b.standardOptimizeOption(.{});

    const raylib = try rl.addRaylib(b, target, optimize, .{});

    const exe = b.addExecutable(.{
        .name = "crafty",
        .target = target,
        .optimize = optimize,
    });
    exe.addCSourceFiles(&.{"src/main.cpp"}, &.{});
    exe.addCSourceFiles(&.{"src/chunks.cpp"}, &.{});
    exe.linkLibCpp();
    exe.linkLibC();
    exe.addIncludePath(.{ .path = src_dir ++ "/raylib/src" });
    exe.addIncludePath(.{ .path = src_dir ++ "/include" });
    exe.addIncludePath(.{ .path = src_dir ++ "/src/base" });
    exe.addIncludePath(.{ .path = src_dir ++ "/src/" });
    exe.linkLibrary(raylib);

    // This declares intent for the executable to be installed into the
    // standard location when the user invokes the "install" step (the default
    // step when running `zig build`).
    b.installArtifact(exe);

    // This *creates* a Run step in the build graph, to be executed when another
    // step is evaluated that depends on it. The next line below will establish
    // such a dependency.
    const run_cmd = b.addRunArtifact(exe);

    // By making the run step depend on the install step, it will be run from the
    // installation directory rather than directly from within the cache directory.
    // This is not necessary, however, if the application depends on other installed
    // files, this ensures they will be present and in the expected location.
    run_cmd.step.dependOn(b.getInstallStep());

    // This allows the user to pass arguments to the application in the build
    // command itself, like this: `zig build run -- arg1 arg2 etc`
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // This creates a build step. It will be visible in the `zig build --help` menu,
    // and can be selected like this: `zig build run`
    // This will evaluate the `run` step rather than the default, which is "install".
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
