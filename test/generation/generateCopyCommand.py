import sys
import os
import json

def generateCopyCommand(manifestPath : str):
    """
    Parameters
    ----------
    manifestPath : str
        The json file path of the manifest
    """

    # read json data into json object
    data = {}
    completeManifestPath = None
    with open(manifestPath) as manifestFile:
        data = json.load(manifestFile)

    # root path
    root = data["dstRoot"]

    # placeholder escape characters since cannot be directly input into f-strings
    newline = "\n"
    backslash = "\\"

    # navigate to root directory
    copyCommand = f"cd {root}{newline}"

    # copy all files
    for item in data["copyManifest"]:
        # make directory for output of copy
        for file in item["inclusions"]:
            inPath = f"{item['src']}/{file}"
            
            # only take the directory as the output path
            lastIdx = -1
            for i in range(len(file) - 1, -1, -1):
                if (file[i] == '/'):
                    lastIdx = i
                    break
            if (lastIdx > 0):
                file = f"/{file}"
            outPath = f"{root}/{item['dst']}{file[:lastIdx+1]}"
            # copy specific file
            copyCommand += f"xcopy \"{inPath.replace('/', backslash)}\" \"{outPath.replace('/', backslash)}\" /i /y{newline}"

    # navigate to current directory after done
    copyCommand += f"cd {os.getcwd()}"

    # write copy command to specified path
    with open("copyFiles.bat", "w") as outFile:
        outFile.write(copyCommand)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Run format: python generateCopyCommand.py <MANIFESTPATH>")
        exit()
    
    generateCopyCommand(sys.argv[1])