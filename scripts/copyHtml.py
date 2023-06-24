import os
import shutil

path = os.path.dirname(os.path.realpath(__file__))
projectRoot = os.path.abspath(os.path.join(path, '..'))

html_directory = os.path.join(projectRoot, 'html')
include_directory = os.path.join(projectRoot, 'include')


# Get a list of HTML files in the html directory
html_files = [file for file in os.listdir(html_directory) if file.endswith('.html')]

for file in html_files:
    # Generate the new file name with .h extension
    new_file_name = os.path.splitext(file)[0] + '.h'
    
    # Read the content of the HTML file
    with open(os.path.join(html_directory, file), 'r') as html_file:
        file_content = html_file.read()
    
    # Create the content to be added at the top and bottom of the file
    include_guard = '_'.join(os.path.splitext(new_file_name)[0].split()).upper()
    header_top = f"#ifndef _{include_guard}\n#define _{include_guard}\n\nconst char *{os.path.splitext(new_file_name)[0].split()[0]} = R\"(\n"
    header_bottom = f"\n)\";\n#endif // _{include_guard}\n"
    
    # Create the new file in the include directory
    with open(os.path.join(include_directory, new_file_name), 'w') as new_file:
        # Write the header content at the top of the file
        new_file.write(header_top)
        
        # Write the HTML file content
        new_file.write(file_content)
        
        # Write the header content at the bottom of the file
        new_file.write(header_bottom)

    print(f"File '{file}' copied and renamed to '{new_file_name}' in the '{include_directory}' directory.")
