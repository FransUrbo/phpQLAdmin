<?php
// {{{ pql_get_os_name(void)
function pql_get_os_name() {
  // TODO: make this more comprehensive for the platforms it will work on.
  preg_match("/(Linux|Win32)/i", $_SERVER['SERVER_SOFTWARE'], $matches);
  $osname = strtolower($matches[1]);
  if($osname == 'linux')
    return 'unix';
  elseif($osname == 'win32')
    return 'windows';
  else
    return 'unix'; // default action
}
// }}}

// {{{ pql_find(path, files, pattern, separator)
function pql_find($path, &$files, $pattern, $separator) {
  $d = array(); $nd = 0;
  $f = array(); $nf = 0;

  $hndl = opendir($path);
  while($file = readdir($hndl)) {
    if($file == '.' || $file == '..')
	  continue;

    if(is_dir($path.$separator.$file))
      $d[$nd++] = $file;
    elseif(preg_match($pattern, $file))
      // Do a file type check at this point
      $f[$nf++] = $file;
  }
  closedir($hndl);

  $n = count($files);
  for($i=0; $i < count($d); $i++)
    find($path.$separator.$d[$i], $files, $pattern, $separator);

  for($i=0; $i < count($f); $i++)
    $files[$n++] = $path.$separator.$f[$i];
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
