import os
import sys as sys
from os import path
from argparse import ArgumentParser
from subprocess import Popen, PIPE
import base64 as b64

API_TO_LANG = {
    "D3D12": "DXIL",
    "VULKAN": "SPIRV",
    "METAL": "MetalLib"
}


class PSOCompileHelper:
    def __init__(self, scar_executable: str, api: str):
        self.scar_executable = scar_executable
        self.lang = API_TO_LANG[api]

    def compile(self, desc_filepath: str, out_filepath: str):
        if path.dirname(out_filepath):
            os.makedirs(path.dirname(out_filepath), exist_ok=True)

        compile_args = [desc_filepath]
        compile_args += ["--O0"]
        compile_args += ["-t", self.lang]

        process = Popen([self.scar_executable] + compile_args, stdout=PIPE, stderr=PIPE)
        output, error = process.communicate()
        process.wait()
        if process.returncode != 0:
            raise Exception(f"Failed to compile PSO. Compiler exited with error:\n{error.decode()}")

        with open(out_filepath, "wb") as out_file:
            out_file.write(b64.b64decode(output))


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--compiler-path", required=True)
    parser.add_argument("--api", required=True, choices=["D3D12", "VULKAN", "METAL"])
    parser.add_argument("--example-id", required=True, choices=["RT", "Compute", "Fractal"])
    parser.add_argument("--out-dir", default="./")


    try:
        args = parser.parse_args()
        shaders_dir = path.abspath(path.join(path.dirname(__file__), "shaders"))
        out_dir = path.abspath(args.out_dir)

        compileHelper = PSOCompileHelper(path.abspath(args.compiler_path), args.api)

        if args.example_id == "RT":
            print("Compiling RT archive")
            compileHelper.compile(path.join(shaders_dir, "rt", "rt.desc.json"), path.join(out_dir, "rt.scar"))
            print("Done compiling RT archive" + path.join(out_dir, "rt.scar"))
        elif args.example_id == "Compute":
            print("Compiling Compute archive")
            compileHelper.compile(path.join(shaders_dir, "compute", "compute.desc.json"),
                                  path.join(out_dir, "compute.scar"))
            print("Done compiling Compute archive: " + path.join(out_dir, "compute.scar"))
        elif args.example_id == "Fractal":
            print("Compiling Fractal archive")
            compileHelper.compile(path.join(shaders_dir, "fractal", "fractal.desc.json"),
                                  path.join(out_dir, "fractal.scar"))
            compileHelper.compile(path.join(shaders_dir, "fractal", "blit.desc.json"),
                                  path.join(out_dir, "blit.scar"))
            print("Done compiling Fractal archive: " + path.join(out_dir, "fractal.scar"))

    except Exception as e:
        print(str(e), file=sys.stderr)
        exit(-1)
