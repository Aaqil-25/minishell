#!/bin/bash
echo "Testing unset functionality..."
echo ""
echo "1. Check if PATH exists initially:"
./minishell << 'MINISHELL_INPUT'
env
MINISHELL_INPUT
