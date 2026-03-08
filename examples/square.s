; x = 4
; square = x * x
; print(square)

pushc 4
store 'x'

pushv 'x'
pushv 'x'
mult
store 'square'
