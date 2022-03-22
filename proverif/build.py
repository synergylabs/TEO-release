#!/usr/bin/env python3
import hydra
from omegaconf import DictConfig, OmegaConf
import re

from compile_utils import *

TEMPLATE_FILE = 'teo-template.pv'
OUTPUT_FILE = 'teo-compiled.pv'


@hydra.main(config_name='compile_config')
def main(cfg: DictConfig):

    var_lookup = {
        'UID': [],
        'REENCRYPT_PHASE_INTERVAL': '',
    }
    for i in range(1, cfg.numUsers+1):
        var_lookup['UID'].append(str(i))
    var_lookup['REENCRYPT_PHASE_INTERVAL'] = str(cfg.reencryptPhaseInterval)

    infile = open(hydra.utils.to_absolute_path(TEMPLATE_FILE), 'r')
    file_in_mem = []
    for line in infile:
        file_in_mem.append(line)
    infile.close()

    ####################################
    # Syntax validation pass
    ####################################

    # Load file into memory
    line_number = 0
    for line in file_in_mem:
        line_number += 1
        extract_command(line, line_number)

    # $template are closed with $endtemplate
    template_state = False
    line_number = 0
    for line in file_in_mem:
        line_number += 1
        cmd = extract_command(line, line_number)
        template_state = update_template_state(cmd, template_state, line_number)

    print('[Progress] Validation pass finished.')

    ####################################
    # Compile pass
    ####################################

    output_in_mem = []
    template_state = False
    template_content = []
    line_number = 0
    for line in file_in_mem:
        line_number += 1
        cmd = extract_command(line, line_number)
        vars = extract_variable(line, line_number)
        template_state = update_template_state(cmd, template_state, line_number)

        if (not template_state) and (EXPAND_CMD not in cmd) and (END_TEMPLATE_CMD not in cmd):
            output_in_mem.append(line)


        line_expanded = line

        if EXPAND_CMD in cmd:
            cmd_arg = {}
            for c in cmd:
                c_idx = re.search(f'\${c}', line_expanded)
                subline = line_expanded[c_idx.end():]
                arg_begin = 1
                arg_end = 0
                layer = 0
                for char in subline:
                    if char == '{':
                        layer += 1
                    elif char == '}':
                        layer -= 1
                        if layer == 0:
                            break
                    arg_end += 1
                cmd_arg[c] = subline[arg_begin:arg_end]

            subject_term = EXPAND_CMD
            subject_idx = re.search(VAR_REGEX, cmd_arg[subject_term])
            if subject_idx is None:
                # expand on keyword provided by $by
                assert EXPAND_BY_CMD in cmd_arg.keys(), f'No term to expand on at line: {line_number}'
                subject_term = EXPAND_BY_CMD
                subject_idx = re.search(VAR_REGEX, cmd_arg[subject_term])
            assert subject_idx is not None, f'Error on expand command at line {line_number}'
            expand_subject = cmd_arg[subject_term][subject_idx.start()+2:subject_idx.end()-1]

            assert EXPAND_WITH_CMD in cmd_arg.keys(), f'No expand concatnation $with at line: {line_number}'
            concat = cmd_arg[EXPAND_WITH_CMD]

            expansion = ''
            for i in range(len(var_lookup[expand_subject])):
                term = cmd_arg[EXPAND_CMD]
                term = term.replace(f'${{{expand_subject}}}', var_lookup[expand_subject][i])
                expansion = expansion +  term + concat
            expansion = expansion[:-len(concat)]

            expanded_line = re.sub(r'\$.*\}', expansion, line)
            if not template_state:
                output_in_mem.append(expanded_line)
            else:
                line_expanded = expanded_line

        if BEGIN_TEMPLATE_CMD in cmd:
            # Enters template state
            assert len(vars) == 1, 'Currently only support one variable within template'

            for var in vars:
                if var not in var_lookup.keys():
                    raise RuntimeError(f'Unknown variable definition: {var}')
                for i in range(len(var_lookup[var])):
                    template_content.append([])
            pass
        elif template_state:
            # Append template commands
            for i in range(len(template_content)):
                line_updated = line_expanded

                if len(cmd) == 0 and len(vars) == 0:
                    # Not variables, simply duplicate code
                    pass
                else:
                    # Adjust for variable value
                    for v in vars:
                        if v in var_lookup.keys():
                            if type(var_lookup[v]) == list:
                                line_updated = line_updated.replace(f'${{{v}}}', f'{var_lookup[v][i]}')
                            else:
                                line_updated = line_updated.replace(f'${{{v}}}', f'{var_lookup[v]}')

                    if FOREACH_CMD in cmd:
                        line_updated = process_foreach(line_updated, var_lookup)

                        pass
                    elif COMPUTE_CMD in cmd:
                        content_m = re.search(r'\$compute\{(.*)\}', line_updated)
                        content = content_m.group(1)
                        sub = str(eval(content))
                        line_updated = re.sub(r'\$compute\{.*\}', sub, line_updated)
                        pass
                template_content[i].append(line_updated)
            pass

        elif END_TEMPLATE_CMD in cmd:
            # Flush template copies
            for entry in template_content:
                output_in_mem.extend(entry)
                output_in_mem.append('\n')
            template_content = []
            pass

    print('[Progress] Compile pass finished.')

    ####################################
    # Final output validation with ProVerif parser
    ####################################
    with open(hydra.utils.to_absolute_path(OUTPUT_FILE), 'w') as out:
        for line in output_in_mem:
            out.write(line)

    print('[Progress] Finish final output.')


if __name__ == '__main__':
    main()
