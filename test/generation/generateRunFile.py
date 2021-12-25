import sys
import os
import json

def generateRunFiles(manifestPath : str, outputFile : str):
    """
    Parameters
    ----------
    manifestPath : str
        The file path of the manifest
    outputFile : str
        The file path of the output executable
    """

    # read json data into json object
    data = {}
    with open(manifestPath) as manifestFile:
        data = json.load(manifestFile)

    root = data["root"]
    runDir = f"{root}/{data['runDir']}"

    dirs = []
    for srcSubDir in data["directories"]:
        dirs.append(f"{root}/{srcSubDir}")

    filePaths = []
    for dir in dirs:
        try:
            for filename in os.listdir(dir):
                if filename.endswith(".c"):
                    filePaths.append(f"{dir}/{filename}")
        except:
            # skip over if invalid directory
            continue

    # placeholder escape characters since cannot be directly input into f-strings
    escape = " ^\n\t"
    
    # write debug command
    with open(f"debug.bat", "w") as debugFile:
        debugFile.write(f"""gcc -g -o {runDir}/{outputFile}{escape}{escape.join(filePaths)}
cd {runDir}
gdb {runDir}/{outputFile}
cd {os.getcwd()}
pause""")

    # write run command
    with open(f"run.bat", "w") as runFile:
        runFile.write(f"""gcc -o {runDir}/{outputFile}{escape}{escape.join(filePaths)}
cd {runDir}
{runDir}/{outputFile}
cd {os.getcwd()}
pause""")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Run format: python generateRunFile.py <MANIFESTPATH> <OUTPUTFILE>")
        exit()
    
    generateRunFiles(sys.argv[1], sys.argv[2])