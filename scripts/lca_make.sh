if [ -z "$1" ]; then
  echo "No file provided"
fi
name="${1/.cpp}.cpp"
{
  echo "%:include \"main.h\""
  echo "SOLVE() {"
  echo "  "
  echo "}"
} > "${name}.tmp"
cp -i "${name}.tmp" "${name}"
rm -f "${name}.tmp"
vim "${name}" +3
