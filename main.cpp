#include "cl_application.h"
int main(){
    cl_application objSystemClass(nullptr);
    objSystemClass.build_tree_objects();
    return objSystemClass.exec_app();
}