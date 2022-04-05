import os
import re

DEBUG = os.getenv('DEBUG', False)

# no unknown commands ($commandName)
BEGIN_TEMPLATE_CMD = 'template'
COMPUTE_CMD = 'compute'
END_TEMPLATE_CMD = 'endtemplate'
EXPAND_BY_CMD = 'by'
EXPAND_CMD = 'expand'
EXPAND_WITH_CMD = 'with'
FOREACH_CMD = 'foreach'
IF_CMD = 'if'

KEYWORDS = [
    '',  # ${VAR} has empty keyword
    BEGIN_TEMPLATE_CMD,
    COMPUTE_CMD,
    END_TEMPLATE_CMD,
    EXPAND_BY_CMD,
    EXPAND_CMD,
    EXPAND_WITH_CMD,
    FOREACH_CMD,
    IF_CMD,
]

ARGUMENT_PREFIX = r'[\{\s]'
KEYWORD_PREFIX = '\$' # Escape special char for RegEx
VAR_PREFIX = '\$\{'
VAR_SUFFIX = '\}'
VAR_REGEX = r'' + VAR_PREFIX + r'[^\$]*' + VAR_SUFFIX

def update_template_state(cmd, template_state, line_number):
    if 'template' in cmd:
        if template_state:
            raise RuntimeError(f'Nested template state not supported at line: {line_number}')
        template_state = True
    elif 'endtemplate' in cmd:
        if not template_state:
            raise RuntimeError(f'End of template but never begin at line: {line_number}')
        template_state = False
    return template_state

def extract_command(line, line_number):
    line = line.rstrip()
    cmd = []
    for i in re.finditer(KEYWORD_PREFIX, line):
        subline = line[i.end():]

        keyword_end = re.search(ARGUMENT_PREFIX, subline)
        if keyword_end is None:
            k = subline
        else:
            k = subline[:keyword_end.start()]

        if k not in KEYWORDS:
            raise RuntimeError(f'Unidentified keyword: {k} in line #{line_number}: {line}')

        if k != '':
            if DEBUG:
                print(f'Command found at {line}: {k}')

            cmd.append(k)

    if DEBUG and len(cmd) > 0:
        print(cmd)
    return cmd

def extract_variable(line, line_number):
    line = line.rstrip()
    vars = []
    for i in re.finditer(VAR_PREFIX, line):
        subline = line[i.end():]
        var_end = re.search(VAR_SUFFIX, subline)
        if var_end is None:
            raise RuntimeError(f'Unmatched variable definition in line #{line_number}: {line}')
        var = subline[:var_end.start()]

        if DEBUG:
            print(f'Variable found at {line}: {var}')

        vars.append(var)

    return vars


def process_foreach(line_in, var_lookup):
    line = line_in
    if '$foreach' not in line:
        return line
    # Expand foreach term
    # m = re.search(r'\$foreach\{([^\{\}]*)\}\{([^\{\}]*)\}\{(.*)\}\$with\{([^\{\}]*)\}', line)
    m = re.search(r'([^\{\}]*)\$foreach\{([^\{\}]*)\}\{([^\{\}]*)\}\{(.*)\}\$with\{([^\{\}]*)\}(.*)', line)
    pre_foreach = m.group(1)
    term = m.group(2)
    subject = m.group(3)
    body_og = m.group(4)
    concat = m.group(5)
    post_foreach = m.group(6)

    print('body_og: ', body_og)
    body = process_foreach(body_og, var_lookup)

    print('line: ', line)
    print('term: ',term)
    print('subject: ', subject)
    print('body: ', body)
    print('concat: ', concat)

    replace = ''
    for sub in var_lookup[subject]:
        body_sub = re.sub(r'\$\{' + term + r'\}', sub, body)
        m_if = re.search(r'(.*)\$if\{(.*)\}\{(.*)\}\{(.*)\}(.*)', body_sub)
        pre_if = m_if.group(1)
        cond = m_if.group(2)
        clause_t = m_if.group(3)
        clause_f = m_if.group(4)
        post_if = m_if.group(5)

        clauss = ''
        if eval(cond):
            clause = clause_t
        else:
            clause = clause_f
        replace = replace + pre_if + clause + post_if + concat
    replace = replace[:-len(concat)]

    pos = re.search(r'[^\$\{\}]*(\$foreach.*\$with\{[^\$\{\}]\}).*', line)
    # print('group: ', pos.group(1))
    print('before: ', line.rstrip())
    # line = re.sub(pos.group(1), replace, line)
    line = line.replace(pos.group(1), replace)
    print('replace: ', replace)
    print('pos: ', pos.group(1))
    print('after: ', line.rstrip())
    print()
    return line
