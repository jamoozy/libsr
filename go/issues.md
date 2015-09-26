# How to Write Go Code

Go is a great language.  One huge improvement over other languages, is Go's attempt to unify Go projects' structures.  There is [an excellent document](https://golang.org/doc/code.html) on [golang.org](https://golang.org) explaining how this unification is meant to take place.

## Workspaces

Unfortunately, I'm not yet fully comfortable with the required [workspace structure](https://golang.org/doc/code.html#Workspace) required for seamless integration with `go build`, `go install`, and `go get`.  For those not familiar with it, the Go assumes that the `$GOPATH` environment variable points to a directory dedicated to your go code, that takes on a specific structure.  On my system, this would mean that `libsr` should be structured as follows:

```
bin/
    libsr-demo                      # (no plans for this, but this would be
                                    #  where any potential Go binaries would
                                    #  reside.)
pkg/
    linux_amd64/
        github.com/jamoozy/libsr
            libsr.a                 # All libsr bindings
            paleo.a                 # Just PaleoSketch bindings.
            dollarp.a               # Just DollarP bindings.
            symbol.a                # Just Ouyang symbol recognizer bindings
src/
    github.com/jamoozy/libsr
        .git/                       # Git repository metadata
        libsr.a                     # All libsr bindings
        paleo.a                     # Just PaleoSketch bindings.
        dollarp.a                   # Just DollarP bindings.
        symbol.a                    # Just Ouyang symbol recognizer bindings
```

Unfortunately, the single directory where this file resides is the only `src/`-like directory for libsr on Go.  There are two solutions to this, neither of which I'm perfectly happy with:
  1. Symlinks via, e.g.,
    ```$ ln -s `pwd` $GOPATH/src/github.com/jamoozy/libsr```
  2. Creating a brand new repository for go bindings.

Since `libsr` is still in pre-alpha, I'm putting off this decision.  I leave it to the reader to set up their environment as required to use `libsr` with Go.
