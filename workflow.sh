#!/bin/zsh

# Ensure 2 arguments are provided
if [[ "$#" -ne 2 ]]; then
    echo "Usage: workflow.zsh <commitmessage> <tag>"
    exit 1
fi

commitmessage="$1"
tag="$2"

# Proper regex for tags like v0.0.0
local tagRE='^v[0-9]+\.[0-9]+\.[0-9]+$'

# Use Zsh's [[ =~ ]] operator for regex matching
if [[ "$tag" =~ ${tagRE} ]]; then
    echo "Version: $tag"
else
    echo "Error: version should be of pattern v0.0.0; invalid version '$tag'"
    exit 1
fi

# Git workflow
git add .
git commit -m "$commitmessage"
git push origin main
git tag "$tag"
git push origin "$tag"
