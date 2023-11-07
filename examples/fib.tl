
let fib = fn(n) {
    if n == 0 { 1 }
    else if n == 1 { 1 }
    else { fib(n - 1) + fib(n - 2) }
}

return fib(10);
