import subprocess

progname = "./rec8"

def ensure_built():
    makey = subprocess.getoutput("make")
    print("calling make... ", end="", flush=True)
    if "Error" in makey:
        print(makey)
        print("\n\nfailed to make; not running any tests.")
        exit()
    print("\tcode built.")

def call_it(*args):
    #return subprocess.getoutput(progname + " " + " ".join(args))
  try:
    return subprocess.check_output(progname + " " + " ".join(args),timeout = 3, shell=True).decode("utf-8")[:-1]
  except subprocess.TimeoutExpired:
    return "timed out!"


def score(ans,*args):
    got = call_it(*args)
    if ans==got:
        print(".",end="",flush=True)
        return 1
    print("\nERROR:\t %s %s\n\texpected: %s\n\tgot:      %s" % (progname, " ".join(args),ans,got))
    return 0

def main():
    ensure_built()
    
    # each test has the expected answer first, then
    # the exact arguments we'd pass to ./rec8.
    tests = [
        ("126","call_or8"),
        ("9","call_to_goal"),
        ("4","call_overwrite"),
        
        ("1", "rec_fib", "1"),
        ("0", "rec_fib", "0"),
        ("5", "rec_fib", '5'),
        ("34", "rec_fib", '9'),
        
        ]    
    
    tally = 0
    for test in tests:
        tally += score(*test)
    
    print("\n\nscore: %s/%s" % (tally,len(tests)))

if __name__=="__main__":
    main()
