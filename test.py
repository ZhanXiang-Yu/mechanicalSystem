from argparse import ArgumentParser
from time import sleep
import sys

parser = ArgumentParser()
parser.add_argument("time", type=int, help="seconds to sleep")
parser.add_argument("--fail", action="store_true", help="exit with code 1 at the end")
parser.add_argument("--syntax-error", action="store_true", help="simulate bad code")
parser.add_argument("--runtime-error", action="store_true", help="raise exception")
args = parser.parse_args()

if args.syntax_error:
    # This is deliberately invalid Python that will cause a SyntaxError,
    # but we cannot *execute* this line directly here because the file must parse.
    # So we simulate by exec'ing a bad string:
    bad_code = "def foo(:\n    pass"
    try:
        exec(bad_code)
    except SyntaxError as e:
        import sys
        print(f"Simulated syntax error: {e}", file=sys.stderr)
        sys.exit(1)

if args.runtime_error:
    print("About to raise a runtime error", file=sys.stderr)
    raise RuntimeError("Intentional runtime error for testing")

print(f"Starting timer of {args.time} seconds")
for _ in range(args.time):
    print(".", end="", flush=True)
    sleep(1)

if args.fail:
    print("\nTimer finished but failing on purpose.", file=sys.stderr)
    sys.exit(1)

print("\nDone!")
sys.exit(0)
