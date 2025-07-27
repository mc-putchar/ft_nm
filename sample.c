const int variable = 42;
const int VARIABLE = 42;
const int Variable = 42;
const int __variable = 42;
const int __variable_ = 42;
const int __VARIABLE = 42;
const int __VARIABLE__ = 42;
const int _var__iable = 42;
const int _var__iable_ = 42;

int main() {
    if (variable == VARIABLE && variable == Variable &&
        variable == __variable && variable == __variable_ &&
        variable == __VARIABLE && variable == __VARIABLE__ &&
        variable == _var__iable && variable == _var__iable_) {
        return 0;
    }
    return 1;
}
