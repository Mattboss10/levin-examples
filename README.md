# Levin examples: slow checks, answered fast and certified

Four tiny formal-verification checks that **do not finish quickly on one machine**, so you
can feel what [Levin](https://mcp.commandersolver.com) does: it splits each check's SAT
formula across many machines, solves the pieces at once, and checks every piece's proof
with a formally verified checker before answering. Your checker (CBMC here) prints its own
`VERIFICATION SUCCESSFUL`, exactly as with a local solver.

Only the numbered SAT formula leaves your machine. No source code, file names or variable
names; formulas with comment lines are refused.

## No CBMC? Try it in 10 seconds

Connect your AI agent to Levin's MCP server (no sign-in) and ask it to call `levin_try`:
a real check, answered and certified by Levin in about ten seconds, with nothing to install.

```
claude mcp add --transport http levin https://mcp.commandersolver.com/mcp
```

Other clients that connect to remote servers:

```json
{ "mcpServers": { "levin": { "url": "https://mcp.commandersolver.com/mcp" } } }
```

Clients that only run local servers can use a bridge:

```json
{ "mcpServers": { "levin": { "command": "npx", "args": ["-y", "mcp-remote", "https://mcp.commandersolver.com/mcp"] } } }
```

## Feel it in one command

You need **CBMC** installed (`cbmc --version`). Then, from the repo root:

```sh
./run.sh verification/pricing.c -DW=8
```

It runs the check locally with a 5-second limit first. That does not finish, so it sends
the formula to Levin and comes back certified. Try the others too:

```sh
./run.sh verification/fmix.c     -DB=16
./run.sh verification/mul_loop.c -DW=10
./run.sh verification/booking.c  -DOPS=10
```

## What it looks like

Measured on Levin's public demo (6 solver machines), 2026-11-25. "Local" is CBMC on one
machine with a 5-second limit; without a limit, CBMC does not finish these within 10
minutes (see Levin's benchmark, `bench/results/speed_cbmc.md`).

| Check | Local, 5 s | Levin | Answer |
|---|---|---|---|
| pricing | times out | 44 s | no bug, certified |
| fmix | times out | 22 s | no bug, certified |
| mul-loop | times out | 66 s | no bug, certified |

## Use it from Claude Code

Install the plugin, then just ask Claude to prove a check and it uses Levin when the check
is slow:

```sh
claude plugin marketplace add Mattboss10/levin-tools
claude plugin install levin-certified-checks@levin-tools
```

Then, in this repo: *"Prove `verification/pricing.c` with CBMC (`-DW=8`)."* Claude runs it
locally first, asks before sending the formula, and returns the certified answer.

## Other MCP clients

Levin's MCP server is `https://mcp.commandersolver.com/mcp` (no sign-in), listed in the
official MCP registry as `com.commandersolver/levin`.

## How `./run.sh` works

`run.sh` runs `cbmc <file> <flags> --function harness --unwind 34 --unwinding-assertions`
with a 5-second local try, then re-runs it with `./levin-sat` as CBMC's external SAT solver.
`levin-sat` is a short script (in this repo) that posts only the numbered formula to
`https://mcp.commandersolver.com/v1/solve` and prints the answer. It writes the job id to
`levin.log`. Nothing else leaves your machine.

Free during the demo, within an hourly allowance per address. Privacy:
https://mcp.commandersolver.com/privacy · Usage: https://mcp.commandersolver.com/stats

MIT licensed.
