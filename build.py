import os
import subprocess
import sys
from pathlib import Path

# Define paths
project_root = Path(__file__).resolve().parent

venv_dir = project_root / ".venv"
python_path = venv_dir / ("Scripts/python" if os.name == "nt" else "bin/python")
requirements_path = project_root / "requirements.txt"

lib_dir = project_root / "lib"

backend_dir = project_root / "backend_module"
cmake_backend_build_dir = backend_dir / "build"
backend_module_dir = lib_dir / "backend_module"

user_interface_dir = project_root / "user_interface"

# Debug prints
print(f"project_root: {project_root}")
print(f"venv_dir: {venv_dir}")
print(f"python_path: {python_path}")
print(f"requirements_path: {requirements_path}")

# Check if in the correct directory
if project_root.name != "wsn":
    print(f"Expected directory 'wsn', but got '{project_root.name}'")
    sys.exit(-1)

# Create virtual environment if it doesn't exist
def create_venv():
    if not venv_dir.exists():
        print("Creating virtual environment...")
        try:
            subprocess.run([sys.executable, "-m", "venv", str(venv_dir)], check=True)
            print("Virtual environment created successfully.")
        except subprocess.CalledProcessError as e:
            print(f"Error creating virtual environment: {e}")
            sys.exit(-1)
    else:
        print("Virtual environment already exists. Skipping creation.")

# Add package path to venv 
def add_package_path_to_venv(package_path):
    if os.name == 'nt':  # Windows
        activate_path = os.path.join(venv_dir, 'Scripts', 'activate.bat')
        line_to_add = f'set PYTHONPATH=%PYTHONPATH%;{package_path}\n'
    elif os.name == 'posix':  # Linux/Mac
        activate_path = os.path.join(venv_path, 'bin', 'activate')
        line_to_add = f'export PYTHONPATH="$PYTHONPATH:{package_path}"\n'
    else:
        raise OSError("Unsupported operating system.")

    # adding path to activate file
    with open(activate_path, 'a') as file:
        file.write(line_to_add)

    print(f"Path {package_path} added to {activate_path}.")

# Activate venv if not active
def activate_venv():
    if "VIRTUAL_ENV" not in os.environ:
        activate_script = venv_dir / ("Scripts/activate" if os.name == "nt" else "bin/activate")
        print(f"activate_script: {activate_script}")  # Debug print
        if activate_script.exists():
            print("Activating virtual environment...")
            if os.name == "nt":
                # Windows
                activate_script = venv_dir / "Scripts" / "activate"
                subprocess.run(f'{activate_script}', shell=True, check=True)
            else:
                # Linux/Mac
                activate_script = venv_dir / "bin" / "activate"
                subprocess.run(f'source {activate_script}', shell=True, check=True)
        else:
            print("Virtual environment not found.")
            sys.exit(-1)

# Install requirements
def install_requirements():
    subprocess.run([str(python_path), "-m", "pip", "install", "--upgrade", "pip"], check=True)
    pip_path = venv_dir / ("Scripts/pip" if os.name == "nt" else "bin/pip")
    if requirements_path.exists():
        print("Installing requirements...")
        subprocess.run([str(pip_path), "install", "-r", str(requirements_path)], check=True)
    else:
        print("requirements.txt not found.")
        sys.exit(-1)

# Configure CMake
def configure_cmake():
    if not cmake_backend_build_dir.exists():
        cmake_backend_build_dir.mkdir()
    print("Configuring CMake...")
    subprocess.run(["cmake", "-S", str(project_root), "-B", str(cmake_backend_build_dir), "-DCMAKE_BUILD_TYPE=Release"], check=True)

# Build project
def build_project():
    print("Building project...")
    subprocess.run(["cmake", "--build", str(cmake_backend_build_dir), "--config", "Release", "--target", "install"], check=True)
        # Build the test executable
    subprocess.run([
        "cmake", "--build", str(cmake_backend_build_dir),
        "--config", "Release", "--target", "cpp_test"
    ], check=True)

# Initialize package
def init_package():
    if not backend_module_dir.exists():
        print("backend_module directory not found")
        sys.exit(-1)

    # Ensure __init__.py exists
    init_file = backend_module_dir / "__init__.py"
    if not init_file.exists():
        print("Creating __init__.py in backend_package...")
        init_file.touch()
    else:
        print("__init__.py already exists. Skipped.")

    # Add backend_package to sys.path
    package_path = str(backend_module_dir.resolve())
    if package_path not in sys.path:
        sys.path.insert(0, package_path)
        print(f"Added {package_path} to sys.path")
        


def generate_pyi():
    python_path = Path(".venv") / ("Scripts/python.exe" if os.name == "nt" else "bin/python")
    module_name = "backend_module"
    output_dir = backend_module_dir
    sys.path.append(str(backend_module_dir))

    print(f"Generating .pyi file for {module_name}...")
    subprocess.run([ 
        str(python_path), "-m", "pybind11_stubgen",
        "--output", str(output_dir),
        module_name
    ], check=True, cwd=str(backend_module_dir))

    print(f".pyi file should be in {output_dir}")

def main():
    try:
        create_venv()
        add_package_path_to_venv(lib_dir)
        activate_venv()
        install_requirements()
        configure_cmake()
        build_project()
        init_package()
        generate_pyi()
        print("Build completed successfully!")
    except subprocess.CalledProcessError as e:
        print(f"Error during execution: {e}")
        sys.exit(-1)

if __name__ == "__main__":
    main()
