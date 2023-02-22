//
// Created by xshx on 2023/2/21.
//

int rename_process(char *new_name){
    int ret=    prctl(PR_SET_NAME, new_name);
    return ret;
}
