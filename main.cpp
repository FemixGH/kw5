#include "systemClass.h"
int main(){
    systemClass objSystemClass(nullptr);
    objSystemClass.build_tree();
    return objSystemClass.start_app();
}