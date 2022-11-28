from SIMPL.run import run

while True:
    text = input('SIMPL > ')
    result, error = run('<stdin>', text)

    if error: print(error.as_string())
    else: print(result)