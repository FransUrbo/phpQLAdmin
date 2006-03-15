<?php
/* OOHForms: file
 *
 * Copyright (c) 1998 by Jay Bloodworth
 *
 * $Id: phplib.of_file.inc,v 1.1.2.1 2006-03-15 09:35:55 turbo Exp $
 */

class of_file extends of_element {

  var $isfile = true;
  var $size;

  function of_file($a) {
    $this->setup_element($a);
  }

  function self_get($val, $which, &$count) {
    $str = "";

    $str .= "<input type='hidden' name='MAX_FILE_SIZE' value=$this->size>\n";
    $str .= "<input type='file' name='$this->name'";
    if ($this->extrahtml) {
      $str .= " $this->extrahtml";
    }
    $str .= ">";

    $count = 2;
    return $str;
  }

} // end FILE
