import os
import sys
import fileinput
import shutil, errno



def copyanything(src, dst):
    try:
        shutil.copytree(src, dst)
    except OSError as exc: # python >2.5
        if exc.errno == errno.ENOTDIR:
            shutil.copy(src, dst)
        else: raise


old_dir = sys.argv[1]

#creates a new copy of the data
walk_dir = "student_"+old_dir
copyanything(old_dir,walk_dir)

solution_dict = {}

print('walk_dir = ' + walk_dir)

# If your current working directory may change during script execution, it's recommended to
# immediately convert program arguments to an absolute path. Then the variable root below will
# be an absolute path as well. Example:
# walk_dir = os.path.abspath(walk_dir)
print('walk_dir (absolute) = ' + os.path.abspath(walk_dir))

for root, subdirs, files in os.walk(walk_dir):
    if (".git" in root): continue
    print('--\nroot = ' + root)
    
    for subdir in subdirs:
        print('\t- subdirectory ' + subdir)

    for filename in files:
        file_path = os.path.join(root, filename)

        if ("__pycache__" in file_path) or \
           (".gc" in filename) or \
           ("SWANKILLER" in filename) or \
           (".o" in filename):
            continue
        print('\t- file %s (full path: %s)' % (filename, file_path))
        CUT = False
        solution_dict[file_path] = ""
        for line in fileinput.input(file_path, inplace=True):
            #preserve leading whitespace
            startwhites = line[:len(line)-len(line.lstrip())]
            #PYTHON
            if ".py" in filename:
                if "#!CUT_END" in line: 
                    CUT = False
                    solution_dict[file_path] += '{}#END CODE HERE\n```\n'.format(startwhites)
                    
                    print('{}#END CODE HERE\n'.format(startwhites), end='')
                elif CUT == True:
                    solution_dict[file_path] += "{} {}".format(fileinput.filelineno(),line) 
                    print('{}\n'.format(startwhites), end='')
                elif "#!CUT_START" in line: 
                    CUT = True
                    solution_dict[file_path] += '``` python\n{}#START CODE HERE\n'.format(startwhites)
                    print('{}#START CODE HERE\n'.format(startwhites), end='')
                elif "#!COMMENT" in line:
                    solution_dict[file_path] += '{} {}\n'.format(fileinput.filelineno(),line.replace("#!COMMENT","")) 
                    print('{}'.format(line.replace("#!COMMENT","#")), end='')
                else:
                    print('{}'.format(line), end='') 
            #C and C++ 
            if (".c" in filename) or (".cpp" in filename):
                if "//!CUT_END" in line: 
                    CUT = False
                    solution_dict[file_path] += '{}//END CODE HERE\n```\n'.format(startwhites)
                elif CUT == True:
                    solution_dict[file_path] += "{} {}".format(fileinput.filelineno(),line) 
                    print('{}\n'.format(startwhites), end='')
                elif "//!CUT_START" in line: 
                    CUT = True
                    solution_dict[file_path] += '``` c\n{}//START CODE HERE\n'.format(startwhites)
                    print('{}//START CODE HERE\n'.format(startwhites), end='')
                elif "//!COMMENT" in line:
                    solution_dict[file_path] += '{} {}\n'.format(fileinput.filelineno(),line.replace("//!COMMENT","")) 
                    print('{}'.format(line.replace("//!COMMENT","//")), end='')
                else:
                    print('{}'.format(line), end='') 
            else: 
                print('{}'.format(line), end='') 

f = open("{}/SOLUTION.md".format(walk_dir), "w")
for item in solution_dict:
    if solution_dict[item] == "":
        continue
    
    f.write("## {}\n".format(item))
    
    f.write(solution_dict[item])
f.close()
