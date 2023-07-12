# 공부한 점 & 느낀 점
## 느낀 점
저자가 코드를 다소 더럽게 작성하는 느낌이 든다. 각자의 코드를 짜는 스타일이나 개성이 있는 것이 당연하지만,
내 개인적인 기준에서는 약간 더럽게 느껴진다. 첫 번째 코드는 책에서 제공한 코드이고, 두 번째는 내가 작성한 코드이다.
```c
auto parseUnary()->Expression* {
    set<Kind> operators = {
        Kind::Add,
        Kind::Subtract,
    };
    while (operators.count(current->kind)) {
        auto result = new Unary();
        result->kind = current->kind;
        skipCurrent();
        result->sub = parseUnary();
        return result;
    }
    return parseOperand();
}

auto skipCurrentIf(Kind token_kind)->bool {
    if (current->kind != token_kind)
        return false;
    current++;
    return true;
}
```
```c
auto parseUnary(std::vector<Token>::iterator & current) -> Expression * {
    using std::set;
    
    set<Kind> operators = { Kind::Add, Kind::Subtract };
    
    return !operators.count(current->kind)
        ? parseOperand(current)
        : new Unary((Unary) {
            .kind = current->kind,
            .sub  = parseUnary(current)
        });
}

auto skipCurrentIf(std::vector<Token>::iterator & current, Kind kind) -> bool {
    return (current += current->kind == kind) - current;
}
```

한가지 배운 점도 있는데, 원래는 전역변수를 전혀 쓰지 않았다. 이 코드를 작성하면서도 전역변수를 사용하지 않았는데 이런 코드는 전역변수를 사용하는 편이 나을 것 같다. 하나의 벡터를 이터레이터로 이동해가면서 이터레이터가 가리키는 요소를 각각의 함수가 처리하는 느낌인데 이터레이터를 무조건 함수의 인자로 넘겨줘야만 한다. 많이 귀찮았다.