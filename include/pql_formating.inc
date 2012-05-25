<?php
// ----------------------------
// pql_formating.inc
// Output formating etc
//
// $Id: pql_formating.inc,v 2.75 2008-03-19 13:24:30 turbo Exp $

// {{{ pql_create_ldif(func, dn, entry, extra_newline, show_debug)
// Create a LDIF object to print in case of error
function pql_create_ldif($func, $dn, $entry, $extra_newline = 0, $show_debug = 1) {
  global $account_type; // TODO: This is ugly (and undocumented!!)
  $command = 0; $LDIF = '';
  
  if($extra_newline)
	$nl = '<br>';
  else
	$nl = '';
  
  if($func)
	$LDIF .= "<br>\n<pre>\n";
  
  if(!file_exists($_SESSION["path"]."/.DEBUG_LDIF") and $show_debug)
	$LDIF .= "--- DEBUG: $func($account_type) ---$nl\n";
  
  if($dn)
	$LDIF .= "dn: ".pql_maybe_decode($dn)."$nl\n";
  elseif($entry["dn"])
	$LDIF .= "dn: ".pql_maybe_decode($entry["dn"])."$nl\n";
  
  if(preg_match('/_save/', $func) or preg_match('/_add/', $func))
	$modify = 0;
  else
	$modify = 1;
  
  if(is_array($entry)) {
	foreach($entry as $key => $e) {
	  if(is_array($entry[$key])) {
		// ... in an array. Go through each one, creating the LDAP entry
		if($func and $modify) {
		  // ------------------------------------
		  // MODIFY
		  if($entry[$key]) {
			if($command)
			  $LDIF .= "-\n";
			
			$LDIF .= "replace: $key$nl\n";
			$command = 1;
			
			foreach($entry[$key] as $x)
			  $LDIF .= "$key: $x$nl\n";
		  } else {
			if($command)
			  $LDIF .= "-\n";
			
			$LDIF .= "delete: $key$nl\n";
			$command = 1;
		  }
		} elseif($key != 'dn') {
		  // ------------------------------------
		  // We're (probably) ADDING an _object_. Print the values as is.
		  foreach($entry[$key] as $x)
			$LDIF .= "$key: ".pql_maybe_encode($x, $key)."$nl\n";
		}
	  } else {
		// ... not an array, create the LDAP entry
		if($func and $modify) {
		  // ------------------------------------
		  // MODIFY
		  if($entry[$key]) {
			if($command)
			  $LDIF .= "-\n";
			
			$LDIF .= "replace: $key$nl\n";
			$command = 1;
			
			$LDIF .= "$key: $entry[$key]$nl\n";
		  } else {
			if($command)
			  $LDIF .= "-\n";
			
			$LDIF .= "delete: $key$nl\n";
			$command = 1;
		  }
		} elseif($key != 'dn') {
		  // ------------------------------------
		  // We're (probably) ADDING an _object_. Print the values as is.
		  $LDIF .= "$key: ".pql_maybe_encode($entry[$key], $key)."$nl\n";
		}
	  }
	}
  }
  
  if(!file_exists($_SESSION["path"]."/.DEBUG_LDIF") and $show_debug)
	$LDIF .= "--- DEBUG ---\n";
  
  if($func)
	$LDIF .= "</pre>\n";
  
  return($LDIF);
}
// }}}

// {{{ pql_complete_constant(text, replacement)
// Replaces dynamic vars in constants %key% will be replaced with $replacement["key"]
function pql_complete_constant($text, $replacement) {
  if(!is_array($replacement))
	return false;
  
  // prepare patterns
  foreach($replacement as $key => $value)
	$pattern[] = "/(%$key%)/i";
  
  return preg_replace($pattern, $replacement, $text);
}
// }}}

// {{{ pql_generate_button(button array)
function pql_generate_button($buttons, $link = '', $url = '') {
  global $LANG, $_REQUEST;
  
  if(empty($url))
	$url = $_SERVER["PHP_SELF"] . "?";
  else
	$url .= '?';
  
  // It's not sure we have a rootdn and/or domain, so only add those
  // it's set
  if(!empty($_REQUEST["rootdn"])) {
	$url .= "rootdn=" . pql_format_urls($_REQUEST["rootdn"]) . "&";
  }
  
  if(!empty($_REQUEST["domain"])) {
	$url .= "domain=" . pql_format_urls($_REQUEST["domain"]) . "&";
  }
?>
									
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th align="left" valign="top">
<?php
  $i=0; // A button counter.
  foreach($buttons as $view => $text) {
	// Generate the button link etc
	if(($view == $_REQUEST["view"]) or ($text == $_REQUEST["cat"]))
	  $mark = "&mark=yes";
	else
	  unset($mark);
?>
      <a href="<?php echo $url?>view=<?php echo $view?><?php if($link) { echo "&$link"; } ?>"><img alt="// <?php echo $LANG->_($text)?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($LANG->_($text)); ?><?php echo $mark?>"></a>
<?php
	// Increase button counter"
    $i++;
  }
?>
    </th>
  </table>

<?php
}
// }}}

// {{{ pql_format_error(output)
// an error-message wrapper for adding special comments to some
// error conditions
function pql_format_error($output) {
  global $_pql;
  
  $errno = ldap_errno($_pql->ldap_linkid);
  $error = ldap_error($_pql->ldap_linkid);
  
  $url = "<a href='config_detail.php?view=".$_pql->ldap_basedn[0]."' target='_new'>Show configuration</a>";
  
  switch($errno) {
  case 17:
	// Undefined attribute type -> invalid schema
	$comment  = "This error is caused by an invalid schema. Have you updated them recently? If not, copy the new schema ";
	$comment .= "that defines this attribute, and restart your LDAP server.";
	break;
	
  case 18:
	// Inappropriate matching --> invalid schema
	$comment = "This error is caused by an invalid schema file.";
	break;
	
  case 21:
	// Invalid syntax
	$comment  = "This error is caused because an attribute is used, but there is no <i>value</i> for it (i.e. an empty ";
	$comment .= "attribute/value pair).<br>";
	$comment .= "The attribute is most likley <b>objectClass</b> and is usually caused because you're missing ";
	$comment .= "objectclasses for this type of object. Go to $url and setup the appropriate <b>objectclasses</b>.";
	break;
	
  case 41:
	// Object Class Violation
	$comment  = "This error is caused because you're using an attribute but haven't included the correct object class.";
	$comment .= "<br>Please go to $url and setup the appropriate <b>objectClasses</b>.";
	break;
  }
  
  $msg = "<p>".pql_format_error_span("LDAP-Error #$errno: $error");
  if($comment)
	$msg .= "<br>$comment<p>";
  
  if($output)
	echo $msg."<br>";
  else
	return $msg;
}
// }}}

// {{{ pql_format_error_span(msg)
function pql_format_error_span($msg) {
  return "<span class=\"error\">" . $msg . "</span>";
}
// }}}

// {{{ pql_format_status_msg(msg, dir)
// dir option is for tools/update_translations.php
// which need 'img src="../images/info.png"'...
function pql_format_status_msg($msg, $dir = NULL) {
?>
  <table cellpadding="3" cellspacing="0" border="0" width="100%">
    <th colspan="3" align="left">
      <tr class="message">
        <td colspan="2" class="message">
          <img src="<?php echo $dir?>images/info.png" width="16" height="16" border="0" align="left">
        </td>

        <td colspan="2" class="message">
          <table>
            <th align="left">
<?php echo stripslashes($msg) . "\n"; ?>
            </th>
          </table>
        </td>
      </tr>
    </th>
  </table>
		
  <br><br>
<?php
}
// }}}

// {{{ pql_format_table(echo = 1)
function pql_format_table($echo = 1) {
  global $flag;
  
  if($flag) {
	$flag = false;
	$ret="c1";
  } else {
	$flag = true;
	$ret="c2";
  }
  
  if($echo) {
	echo $ret;
  } else {
	return $ret;
  }
}
// }}}

// {{{ pql_format_table_single(text, url, array, attribute, class, spaces)
function pql_format_table_single($text, $url, $array, $attrib, $class = 0, $spaces = 0) {
  global $_pql;
  
  // Stop PHP for nagging about undefined index.
  $url["mod"] = (isset($url["mod"])) ? $url["mod"] : 0;
  $url["del"] = (isset($url["del"])) ? $url["del"] : 0;
  $url["add"] = (isset($url["add"])) ? $url["add"] : 0;
  
  if(!empty($array) and !is_array($array))
	$array = array($array);
  
  $space = "";
  if($spaces > 0) {
	for($i=0; $i < $spaces; $i++)
	  $space .= "&nbsp;";
  }
  
  $title_shown = 0;
  if(is_array($array)) { 
	$new_tr = 0;
	asort($array);
	foreach($array as $value) {
	  if(!$class)
		$class  = pql_format_table(0);
	  
	  if(preg_match('/[a-z]=[a-z]/i', $value)) {
		// Hopfully this is a DN
		$username = $_pql->get_attribute($value, pql_get_define("PQL_ATTR_CN"));
		if(!$username[0])
		  $username = $value;
		elseif(is_array($username))
		  $username = $username[0];
	  } else
		$username = $value;
	  
	  if($new_tr) {
  ?>
 
  <tr class="<?php echo $class?>">
    <td class="title"></td>
<?php } else { ?>
  <tr class="<?php echo $class?>">
    <td class="title"><?php echo $text["tit"]?>:</td>
<?php $title_shown = 1;
	  }
	  
	  $new_tr = 1;
?>
    <td class="<?php echo $class?>"><?php echo $space?><?php echo $username?></td>
    <td class="<?php echo $class?>" align="right">
<?php if($url["mod"]) { ?>
      <a href="<?php echo $url["mod"]?>&<?php echo $attrib?>=<?php echo urlencode($value)?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $text["mod"]?>"></a>
<?php }

	  if($url["del"]) {
 ?>
      <a href="<?php echo $url["del"]?>&<?php echo $attrib?>=<?php echo urlencode($value)?>"><img src="images/del.png"  width="12" height="12" border="0" alt="<?php echo $text["del"]?>"></a>
<?php } ?>
    </td>
   </tr>
<?php
	}
  }

  if(!$class) {
?>
  <tr class="<?php echo pql_format_table(0)?>">
<?php } else { ?>
  <tr class="<?php echo $class?>">
<?php } ?>
<?php if(!$title_shown) { ?>
    <td class="title"><?php echo $text["tit"]?>:</td>
<?php } else { ?>
   <td class="title"></td>
<?php } ?>
    <td colspan="2"><?php echo $space?><?php if($url["add"]) { ?><a href="<?php echo $url["add"]?>"><?php echo $text["add"]?></a><?php } ?></td>
  </tr>
<?php
}
// }}}

// {{{ pql_format_table_empty(tds)
function pql_format_table_empty($tds) {
  for($i=0; $i < $tds; $i++) {
	echo "<th>\n";
	echo "  <tr>\n";
	echo "  </tr>\n";
	echo "</th>\n\n";
  }
}
// }}}

// {{{ pql_format_tree(text, url, links, level)
function pql_format_tree($text, $url, $links = '', $level = 0) {
  global $div_counter; // TODO: This is ugly (and undocumented!!)
  
  if(empty($div_counter))
	$div_counter = 0;
  
  $child  = $div_counter . "Child";
  $parent = "Parent";
  
  if($_SESSION["opera"])
	pql_format_tree_opera($div_counter, $text, $url, $level, $parent, $child);
  else
	pql_format_tree_other($div_counter, $text, $url, $level, $parent, $child);
  
  if(is_array($links))
	pql_format_tree_span($text, $links, $level);
  
  if($level)
	pql_format_tree_end();
  
  $div_counter++;
}
// }}}

// {{{ pql_format_tree_opera(counter, text, url, level, parent, child)
// In tables/domain_details-automount.inc, the list is using a five column
// table. To be able to use this function in such a cenario, we need to call
// this function with specific child counters. That's why the last param can
// be an array. If it is, extract the information about the childs from that
// array...
function pql_format_tree_opera($cnt, $text, $url, $level, $parent, $child) {
  $nbsp = '';
  
  if(is_array($child)) {
	$child_nr = ($child[0] * 2) + 1;
	
	$cnt1 = $cnt+1;
	$mouse["click"] = 'onclick="';
	for($i=1; $child[$i]; ) {
	  $mouse["click"] .= 'showhide(el' . $cnt.$child[$i] . ', el' . $cnt.$child[$i+1];
	  if($noicon)
		$mouse["click"] .= ', true';
	  $mouse["click"] .= ');';
	  
	  if($child[$i+2])
		$mouse["click"] .= ' ';
	  
	  $i += 2;
	}
	$mouse["click"] .= '"';
	
	$mouse["over"]  = 'onmouseover="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#CCFFCC\'); hilightBase(\'el'.$cnt1.'\', \'#CCFFCC\')}"';
	$mouse["out"]   = 'onmouseout="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#D0DCE0\'); hilightBase(\'el'.$cnt1.'\', \'#D0DCE0\')}"';
  } else {
	$mouse["click"] = 'onclick="showhide(el'.$child.', el'.$cnt.'Img)"';
	$mouse["over"]  = 'onmouseover="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#CCFFCC\')}"';
	$mouse["out"]   = 'onmouseout="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#D0DCE0\')}"';
  }
  
  if($level) {
	for($i=0; $i < ($level * 4); $i++) 
	  $nbsp .= "&nbsp;";
  }
?>
  <div id="el<?php echo $cnt?><?php echo $parent?>" class="parent"<?php echo "\n\t\t".$mouse["over"]."\n\t\t".$mouse["out"]."\n\t\t".$mouse["click"]?>>
<?php if(is_array($child)) { ?>
    <?php echo $nbsp?><img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?php echo $cnt.$child[$child_nr+1]?>">
<?php } else { ?>
    <?php echo $nbsp?><img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?php echo $cnt?>Img">
<?php } ?>
<?php if($url) { ?><a href="<?php echo $url?>"><?php } ?><font color="black" class="heada"><?php echo $text?></font><?php if($url) { ?></a><?php } ?>
  </div>

<?php if(is_array($child)) { ?>
  <span id="el<?php echo $cnt.$child[$child_nr]?>" style="display:''">
<?php } else { ?>
  <span id="el<?php echo $child?>" style="display:''">
<?php }
}
// }}}

// {{{ pql_format_tree_other(counter, text, url, level, parent, child)
function pql_format_tree_other($cnt, $text, $url, $level, $parent, $child) {
  $nbsp = '';
  
  $mouse["over"]  = 'onmouseover="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#CCFFCC\')}"';
  $mouse["out"]   = 'onmouseout="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#D0DCE0\')}"';
  
  if(is_array($child))
	$child_nr = ($child[0] * 2) + 1;
  
  if($level) {
	for($i=0; $i < ($level * 4); $i++) 
	  $nbsp .= "&nbsp;";
  }
  
  if($url) {
	if(preg_match('/^http/i', $url))
	  $url = "href=\"$url\" target=\"_top\"";
	else
	  $url = "href=\"$url\"";
  }
?>
  <div id="el<?php echo $cnt?>Parent" class="parent" <?php echo $mouse["over"]." ".$mouse["out"]?>>
    <nobr>
      <a class="item" <?php echo $url?> onclick="if (capable) {expandBase('el<?php echo $cnt?>', true); return false;}">
        <?php echo $nbsp?><img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?php echo $cnt?>Img">
      </a>

      <a class="item" <?php echo $url?> onclick="if (capable) {expandBase('el<?php echo $cnt?>', false)}">
        <span class="heada"><?php echo $text?></span>
      </a>
    </nobr>
  </div>

<?php if(is_array($child)) { ?>
  <div id="el<?php echo $child[$child_nr]?>" class="child" style="margin-bottom: 5px" <?php echo $mouse["over"]." ".$mouse["out"]?>>
<?php } else { ?>
  <div id="el<?php echo $child?>" class="child" style="margin-bottom: 5px" <?php echo $mouse["over"]." ".$mouse["out"]?>>
<?php }
}
// }}}

// {{{ pql_format_tree_span(text, links, level)
function pql_format_tree_span($text, $links, $level) {
  $nbsp = '';
  for($i=0; $i < ((($level + 1) * 4) - 1); $i++)
	$nbsp .= "&nbsp;";
  
  $count = count($links); $counter = 0;
  foreach($links as $text => $link) {
	// If it's a user, we want an icon before the list...
	// TODO: An option to the function instead?
	$icon = 0;
	if(preg_match("/user_detail\.php/i", $link) or preg_match("/ezmlm_detail\.php/i", $link))
	  $icon = 1;
	
	if($text) {
	  // {{{ $text is set
	  if($link) {
		// {{{ $link is set
?>
  <nobr>
    <?php echo $nbsp?>

<?php	if(is_array($link)) { ?>
    <input type="checkbox" name="<?php echo $link[0]?>_<?php echo $link[1]?>" value="<?php echo $link[2]?>"><?php echo $text?>
<?php	} else { ?>
<?php if($icon) { ?><img src="images/mail_small.png" border="0" alt="<?php echo $text?>">&nbsp;<?php } ?>
    <a class="tblItem" title="<?php echo $text?>" href="<?php echo $link?>" <?php if(preg_match('/^http:\/\//i', $link)) { echo 'target="_new"'; } else { echo 'target="pqlmain"'; } ?>><?php echo $text?></a>
<?php	} ?>
  </nobr>
  <br />
<?php
// }}}
	  } else {
		  // {{{ $link unset
?>
  <nobr>
    <?php echo $nbsp?>
    <?php if($icon) { ?><img src="images/mail_small.png" border="0" alt="<?php echo $text?>">&nbsp;<?php } ?>
    <?php echo $text?>
  </nobr>
  <br />
<?php }
// }}}
// }}}
	} else {
	  // {{{ $text UNSET
?>
  <nobr>
    <?php echo $nbsp?>
	---------------
  </nobr>
  <br />
<?php
// }}}
	}
	
	$counter++;
  }
}
// }}}

// {{{ pql_format_tree_end(void)
function pql_format_tree_end() {
  if($_SESSION["opera"])
	echo "          </span>\n";
  else
	echo "          </div>\n";
}
// }}}

// {{{ pql_left_htmlify_userlist(rootdn, domain, subbranch, users, &links)
function pql_left_htmlify_userlist($rootdn, $domain, $subbranch, $users, &$links) {
  global $_pql;
  
  // {{{ Iterate trough all users in this domain/branch to find a suitable reference value
  for($i=0; $i < count($users); $i++) {
	$dn = $users[$i];
	unset($cn); unset($sn); unset($gecos);
	
	// {{{ 1. Get the commonName
	$cn = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_CN"));
	if($cn) {
	  // We have a commonName - split it up into two parts (which should be first and last name)
	  if(is_array($cn))
	    $cn = explode(" ", $cn[0]);
	  else
	    $cn = explode(" ", $cn);
	  
	  if(!$cn[1]) {
	    // Don't have second part (last name) of the commonName - MUST be a system 'user'.
	    $cns[$dn] = "System - ".$cn[0];
	  } else {
	    // We have two parts (or more) - combine into 'Lastname, Firstname'
	    // Do this in a for loop so that we can convert something like
	    // 'Test User 3' into '3, Test User'... This sucks for testing, but
	    // it's true :)
	    //
	    // The idea is that the very _last_ part is the last name, and all
	    // the other is firstname(s). This won't hold true for arabic/chinese
	    // names for example (not that I know of any way)...
		
	    // Arrays always start with 0, but count() doesn't.
	    $count = count($cn) - 1;
		
	    // This is/should be the last name
	    $cns[$dn] = $cn[$count].', ';
		
	    // Add (all) the first name(s)
	    for($j=0; $j < $count; $j++) {
	      $cns[$dn] .= $cn[$j];
		  
	      if($cn[$j+1])
			// More first name(s), separate with space...
			$cns[$dn] .= ' ';
	    }
	  }
	} else {
	  // }}}
	  
	  // {{{ 2. No commonnName. Get the givenName and SN.
	  $cn = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_GIVENNAME"));
	  $sn = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_SN"));
	  if($cn && $sn) {
	    // We have a givenName (first name) and a surName (last name) - combine the two
	    if($sn != '_') {
	      if(is_array($sn))
			$cns[$dn] = $sn[0].", ".$cn;
	      else
			$cns[$dn] = $sn.", ".$cn;
	    } else {
	      $cns[$dn] = $cn;
	    }
	  } else {
	    // }}}
		
		// {{{ 3. No givenName, surName or commonName. Get the gecos
	    $gecos = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_GECOS"));
	    if($gecos)
	      // We have a gecos - use that as is
	      $cns[$dn] = $gecos;
	    else {
	      // }}}
		  
		  // {{{ 4. No gecos either. Last chance - use the user reference attribute
	      $ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn);
	      $cns[$dn] = $_pql->get_attribute($dn, $ref);
	    }
	  }
	}
	// }}}
  }
  // }}}
  
  if(is_array($cns)) {
	asort($cns);
	
	if($links != NULL) {
	  foreach($cns as $dn => $cn) {
		if(is_array($cn) and $cn[1])
		  // Multiple CN's - take the first one (might not be the first, but...)
		  $cn = $cn[0];

		// Only get these two so that we don't show root user(s)!
		$uid   = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_UID"));
		$uidnr = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_QMAILUID"));
		
		if(($uid != 'root') or ($uidnr != '0')) {
		  // Do NOT show root user(s) here! This should (for safty's sake)
		  // not be availible to administrate through phpQLAdmin!
		  if($subbranch)
			$new = array($cn => "user_detail.php?rootdn=$rootdn&domain=$domain&subbranch=$subbranch&user=".urlencode($dn));
		  else
			$new = array($cn => "user_detail.php?rootdn=$rootdn&domain=$domain&user=".urlencode($dn));
		  
		  // Add the link to the main array
		  $links = $links + $new;
		}
	  }
	}
	
	return($cns);
  }
}
// }}}

// {{{ pql_format_timestamp_human(beg, end)
function pql_format_timestamp_human($beg, $end = NULL) {
  global $LANG;
  
  if($end)
	$seconds = $end - $beg;
  else {
	// tables/config_details-ppolicy.inc - I already know number
	// of seconds (in the '$beg' variable) - translate this to
	// 'human readable' text
	$seconds = $beg;
  }

  // {{{ Get number of YEARS of uptime
  $tmp  = round($seconds / (60 * 60 * 24 * 7 * 52)); // => 31449600
  if($tmp)
	$uptime['years'] = $tmp;

  if($uptime['years']) {
	$rest = $uptime['years'] * 60 * 60 * 24 * 7 * 52;
	if($rest > 0)
	  $seconds -= $rest;
  }
  // }}}

  // {{{ Get number of WEEKS of uptime
  if($seconds > 0) {
	$tmp  = round($seconds / (60 * 60 * 24 * 7)); // => 604800
	if($tmp)
	  $uptime['weeks'] = $tmp;
	
	if($uptime['weeks']) {
	  $rest = $uptime['weeks'] * 60 * 60 * 24 * 7;
	  if($rest > 0)
		$seconds -= $rest;
	}
  }
  // }}}

  // {{{ Get number of DAYS of uptime
  if($seconds > 0) {
	$tmp  = round($seconds / (60 * 60 * 24)); // => 86400
	if($tmp)
	  $uptime['days'] = $tmp;
	
	if($uptime['days']) {
	  $rest = $uptime['days'] * 60 * 60 * 24;
	  if($rest > 0)
		$seconds -= $rest;
	}
  }
  // }}}
  
  // {{{ Get number of HOURS of uptime
  if($seconds > 0) {
	$tmp = round($seconds / (60 * 60)); // => 3600
	if($tmp)
	  $uptime['hours'] = $tmp;
	
	if($uptime['hours']) {
	  $rest = $uptime['hours'] * 60 * 60;
	  if($rest > 0)
		$seconds -= $rest;
	}
  }
  // }}}
  
  // {{{ Get number of MINUTES of uptime
  if($seconds > 0) {
	$tmp = round($seconds / 60);
	if($tmp)
	  $uptime['minutes'] = $tmp;

	if($uptime['minutes']) {
	  $rest = $uptime['minutes'] * 60;
	  if($rest > 0)
		$seconds -= $rest;
	}
  }
  // }}}
  
  // {{{ Get number of SECONDS of uptime
  if($seconds > 0)
	$uptime['seconds'] = $seconds;
  // }}}
  
  // {{{ Prettify the uptime
  // {{{ YEARS
  if($uptime['years']) {
	if($uptime['years'] > 1)
	  $string = $LANG->_('years');
	else
	  $string = $LANG->_('year');

	$uptime_string['years'] = $uptime['years']." ".$string;

	if($uptime_string['weeks'])
	  $uptime_string['years'] .= ", ";
  }
  // }}}

  // {{{ WEEKS
  if($uptime['weeks']) {
	if($uptime['weeks'] > 1)
	  $string = $LANG->_('weeks');
	else
	  $string = $LANG->_('week');

	$uptime_string['weeks'] = $uptime['weeks']." ".$string;

	if($uptime['days'])
	  $uptime_string['weeks'] .= ", ";
  }
  // }}}

  // {{{ DAYS
  if($uptime['days']) {
	if($uptime['days'] > 1)
	  $string = $LANG->_('days');
	else
	  $string = $LANG->_('day');

	$uptime_string['days'] = $uptime['days']." ".$string;

	if($uptime['hours'])
	  $uptime_string['days'] .= ", ";
  }
  // }}}

  // {{{ HOURS
  if($uptime['hours']) {
	if($uptime['hours'] > 1)
	  $string = $LANG->_('hours');
	else
	  $string = $LANG->_('hours');

	$uptime_string['hours'] = $uptime['hours']." ".$string;

	if($uptime['minutes'])
	  $uptime_string['hours'] .= ", ";
  }
  // }}}

  // {{{ MINUTES
  if($uptime['minutes']) {
	if($uptime['minutes'] > 1)
	  $string = $LANG->_('minutes');
	else
	  $string = $LANG->_('minute');

	$uptime_string['minutes'] = $uptime['minutes']." ".$string;

	if($uptime['seconds'])
	  $uptime_string['minutes'] .= ", ";
  }
  // }}}

  // {{{ SECONDS
  if($uptime['seconds']) {
	if($uptime['seconds'] > 1)
	  $string = $LANG->_('seconds');
	else
	  $string = $LANG->_('second');

	$uptime_string['seconds'] = $uptime['seconds']." ".$string;
  }
  // }}}
  
  $string  = $uptime_string['years'].$uptime_string['weeks'].$uptime_string['days'];
  $string .= $uptime_string['hours'].$uptime_string['minutes'].$uptime_string['seconds'];
  // }}}
  
  return($string."\n");
}
// }}}

// {{{ pql_format_return_unixtime(void)
// Return a UNIX time stamp
//
// Very good for profiling (once before and once after function
// you'd like to profile):
//	$now = pql_format_return_unixtime();
//	echo "Now: (FUNCTION)'$now'<br>";
function pql_format_return_unixtime() {
  $now = date("YhdHis");
  return(pql_format_timestamp_unixtime($now));
}
// }}}

// {{{ pql_format_timestamp_unixtime(date)
// Input: YYYYMMDD[[HHMM]SS]
function pql_format_timestamp_unixtime($date) {
  $date = preg_replace('/Z$/', '', $date);
  
  $year   = $date[0].$date[1].$date[2].$date[3];
  $month  = $date[4].$date[5];
  $day    = $date[6].$date[7];
  
  if(isset($date[8]) and isset($date[9]))
	$hour   = $date[8].$date[9];
  else
	// Not called with hour, fake it.
	$hour   = "00";
  
  if(isset($date[10]) and isset($date[11]))
	$minute = $date[10].$date[11];
  else
	// Not called with minute, fake it.
	$minute = "00";
  
  if(isset($date[12]) and isset($date[13]))
	$second = $date[12].$date[13];
  else
	// Not called with seconds, fake it.
	$second = "00";
  
  return(mktime($hour, $minute, $second, $month, $day, $year));
}
// }}}

// {{{ pql_format_timestamp(timestamp)
function pql_format_timestamp($timestamp) {
  $date   = date("r", pql_format_timestamp_unixtime($timestamp));
  return($date);
}
// }}}

// {{{ pql_format_urls(string)
function pql_format_urls(&$string) {
  if(preg_match('/%3D/', $string))
	// The original string should be URL decoded
	$string = urldecode($string);
  $tmp = $string;
  
  // Make sure we can have a ' in branch (also affects the user DN).
  if(preg_match("/\'/", $tmp)) {
	if(preg_match("/\\\'/", $tmp))
	  // The string already contains \', remove it
	  $tmp = ereg_replace("\\\'", "'", $tmp);
	else
	  $tmp = ereg_replace("\'", "'", $tmp);
	
	// Update the ORIGINAL string!
	$string = $tmp;
  }
  
  // URL encode the value
  $tmp = urlencode($tmp);
  
  return($tmp);
}
// }}}

// {{{ pql_format_bool(string)
function pql_format_bool(&$value) {
  if(is_array($value))
	return;

  if(preg_match('/^true$/i', $value)) {
	settype($value, "boolean");
	$value = '1';
  } elseif(preg_match('/^false$/i', $value)) {
	settype($value, "boolean");
	$value = '0';
  } elseif($value == '1') {
	settype($value, "boolean");
	$value = 'TRUE';
  }
  
  return($value);
}
// }}}

// {{{ pql_format_normalize_dn(dn)
// 1. URL decode the DN
// 2. Lower case DN
// 3. Remove any spaces _after_ commas
//    dn: uid=administrator, ou=People, dc=domain,dc=tld
//	  =>
//	  dn: uid=administrator,ou=People,dc=domain,dc=tld
// 4. Encode the string with international characters
function pql_format_normalize_dn($dn) {
  $dn = urldecode($dn);
  $dn = strtolower($dn);
  $dn = preg_replace('/, /', ',', $dn);
  $dn = pql_maybe_encode($dn);
  
  return($dn);
}
// }}}

// {{{ pql_parse_quota(quota_string)
function pql_parse_quota($quota_string) {
  $quota = '';
  
  // get max bytes
  preg_match("/([0-9]*)S/", $quota_string, $matches);
  if(isset($matches[1])) {
	$quota["maxsize"] = intval($matches[1] / 1024);
	
	// get max mails
	preg_match("/([0-9]*)C/", $quota_string, $matches);
	$quota["maxmails"] =  $matches[1];
  } else
	// The quota value isn't correct. Whing it!
	// Asume that the value in there is SIZE, and set AMOUNT to 0
	// ('should mean' unlimited if I remember correctly).
	return(array($quota_string, "0"));
  
  return $quota;
}
// }}}

// {{{ pql_ldap_mailquota(quota, newline)
// Returns human readable version of users quota
// NOTE: I can remember that there's ONE place where
//       one does NOT want the newline. But I can't remember
//		 where, so this is TRUE by default. I'll deal with
//		 that single instance when I find it...
// UPDATE: Right! It's when the message "Successfully changed quota ..."
//		   is shown. Then we want a comma instead...
function pql_ldap_mailquota($quota, $newline = 1) {
  global $LANG;
  
  if($quota["maxmails"] == "")
	$txt = $LANG->_('Unlimited amount');
  else
	$txt = pql_complete_constant($LANG->_('Max %maxmails% mails'), array("maxmails" => $quota["maxmails"]));
  
  // The HTML newline
  if($newline)
	$txt .= "<br>";
  else
	$txt .= ", ";
  
  if($quota["maxsize"] == "")
	$txt .= $LANG->_('Unlimited size');
  else
	$txt .= pql_complete_constant($LANG->_('Max %maxsize% KB'), array("maxsize" => $quota["maxsize"]));
  
  return $txt;
}
// }}}

// {{{ pql_strip_domain(email)
// Get the username of an email address
function pql_strip_domain($email) {
  $email = explode("@", $email);
  
  if(count($email) == 2)
	return $email[0];
  
  return false;
}
// }}}

// {{{ pql_strip_username(email)
// Get the domainname of an email address
function pql_strip_username($email) {
  $email = explode("@", $email);
  
  if(count($email) == 2)
	return $email[1];
  
  return false;
}
// }}}

// {{{ pql_separate_host_domain(fqdn)
// Separate the domainname and hostname from the FQDN
function pql_separate_host_domain($fqdn) {
  $fqdn = ereg_replace(':.*', '', $fqdn); // Remove any port
  $tmp = explode('\.', $fqdn);
  
  $cnt = count($tmp)-1;
  
  // Domain name is/should be the last two parts
  $domainname = $tmp[$cnt-1].'.'.$tmp[$cnt];
  unset($tmp[$cnt-1]); unset($tmp[$cnt]);
  
  // Hostname is everything UP TO the domainname
  $hostname = '';
  for($i=0; $i < $cnt-1; $i++) {
	$hostname .= $tmp[$i];
	if($tmp[$i+1])
	  $hostname .= ".";
  }
  
  return(array($hostname, $domainname));
}
// }}}

// {{{ pql_uniq(array)
function pql_uniq($array) {
  if(is_array($array)) {
	asort($array);
	$array = array_values(array_unique($array));
  }
  
  return($array);
}
// }}}

// {{{ printr(string)
function printr($string) {
  echo "<pre>\n";
  
  if(is_array($string))
	print_r($string);
  else
	echo "$string<br>";
  
  echo "</pre><p>\n";
}
// }}}

// {{{ lc(string)
// I'm a perlhacker really, does it show!? :)
function lc($string) {
  return(strtolower($string));
}
// }}}

// --------------------------------------
// API functions - Internationalization
// --------------------------------------

// {{{ pql_format_international(string)
// Force a non-internationalization of a string
// (for directories and files for example)
function pql_format_international($string) {
  $new = strtr($string,
			   "ÁÉÍÓÚÑÀÈÌÒÙÅÄËÏÖÜÂÊÎÔÛáéíóúñàèìòùåäëïöüâêîôûç",
			   "AEIOUNAEIOUAAEIOYAEIOUaeiounaeiouaaeioyaeiouc");

  return($new);
}
// }}}

// {{{ pql_maybe_encode(string, attrib, linkid)
// Check if we SHOULD encode the string...
function pql_maybe_encode($string, $attrib = '', $linkid = '') {
  //$old = $string;
  $old = utf8_decode($string);
  
  $new = pql_format_international($old);
  if($old != $new) {
	// The two strings isn't equal -> something have been replaced
	
	if(($attrib != '') && ($linkid != '')) {
	  // Retreive all EXISTING attributes the LDAP server knows about
	  $ldap_attribs = pql_get_subschema("attributetypes");
	  
	  // Find the syntax for the attribute (or it's superior attribute)
	  $syntax = '';
	  if($ldap_attribs[$attrib]["SYNTAX"]) {
		$syntax = $ldap_attribs[$attrib]["SYNTAX"];
	  } else {
		// No syntax, check the SUP if that have a SYNTAX
		// TODO: Be recursive!
		if($ldap_attribs[$attrib]["SUP"] && $ldap_attribs[$ldap_attribs[$attrib]["SUP"]]["SYNTAX"]) {
		  $syntax = $ldap_attribs[$ldap_attribs[$attrib]["SUP"]]["SYNTAX"];
		}
	  }
	  
	  if($syntax) {
		// Does this attribute allow a UTF-8 encoded string?
		// 1.3.6.1.4.1.1466.115.121.1.15	UTF-8 encoded string
		// 1.3.6.1.4.1.1466.115.121.1.41	? - not listed...
		//		Only attributes that uses .41 are:
		//		postalAddress, registeredAddress and homePostalAddress
		//		-> I hope that it's ok to UTF-8 encode these values.
		//		   The database (OpenLDAP 2.0) accepts it anyway.
		if(preg_match('/1\.3\.6\.1\.4\.1\.1466\.115\.121\.1\.15/', $syntax) or
		   preg_match('/1\.3\.6\.1\.4\.1\.1466\.115\.121\.1\.41/', $syntax))
		  return(utf8_encode($old));
		else
		  // Oups, no! Replace any national character with it's
		  // international (ie US char) counter part...
		  return($new);
	  } else
		return($new);
	} else
	  return(utf8_encode($old));
  }
  
  return($old);
}
// }}}

// {{{ pql_maybe_decode(string)
// Check if we SHOULD decode the string...
function pql_maybe_decode($string) {
  if(preg_match("/\=$/", $string) and !preg_match('/^{/', $string))
	// It's a BASE64 string, decode it
	$decoded = base64_decode($string);
  else
	// It seems quite safe to UTF8 decode a non UTF8 string
	$decoded = utf8_decode($string);
  
  if($decoded)
	return($decoded);
  else
	return($string);
}
// }}}

// {{{ pql_maybe_idna_encode(string)
function pql_maybe_idna_encode($string) {
  $old = $string;
  
  $new = pql_format_international($old);
  if($old != $new) {
	// The two strings isn't equal -> something have been replaced
	
	if(function_exists("idn_to_ascii")) {
	  if(preg_match("/@/", $old)) {
		$domain = idn_to_ascii(pql_strip_domain($old));
		
		return(pql_strip_domain($old)."@".$domain);
	  } else
		return(idn_to_ascii($old));
	} else
	  // IDNA isn't availible, return the modified string
	  return($new);
  } else
	// No change - return original string
	return($string);
}
// }}}

// {{{ pql_maybe_idna_decode(string)
function pql_maybe_idna_decode($string) {
  if(preg_match("/^xn--/i", $string) and function_exists("idn_to_utf8")) {
	// It's an IDNA string, and we have the IDN module loaded
	
	// Catch email addresses (can't decode the @)
	if(preg_match("/@/", $string)) {
	  $domain = pql_maybe_decode(idn_to_utf8(pql_strip_username($old)));
	  
	  return(pql_strip_domain($old)."@".$domain);
	} else 
	  // First IDNA decode it, then UTF-8 decode it
	  return(pql_maybe_decode(idn_to_utf8($string)));
  } else {
	return(pql_maybe_decode($string));
  }
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
