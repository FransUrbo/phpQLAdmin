<?php
// ----------------------------
// pql_formating.inc
// Output formating etc
//
// $Id: pql_formating.inc,v 2.6 2003-11-26 16:45:59 turbo Exp $

// {{{ pql_create_ldif(func, dn, entry)
// Create a LDIF object to print in case of error
function pql_create_ldif($func, $dn, $entry) {
	global $account_type;

	if($func)
	  $LDIF .= "<br>--- DEBUG: $func($account_type) ---<br>";

	$LDIF .= "dn: $dn<br>";
	foreach($entry as $key => $e) {
		if(is_array($entry[$key])) {
			// ... in an array. Go through each one, creating the LDAP entry
			foreach($entry[$key] as $x)
			  $LDIF .= "$key: $x<br>";
		} else {
			$LDIF .= "$key: $entry[$key]<br>";
		}
	}

	if($func)
	  $LDIF .= "--- DEBUG ---<p>";	

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

// {{{ pql_ldap_error(ldap_linkid, attrib)
function pql_ldap_error($ldap_linkid, $attrib = "") {
    // an error-message wrapper for adding special comments to some
    // error conditions
    
    $errno = ldap_errno($ldap_linkid);
    $error = ldap_error($ldap_linkid);
    
    switch($errno){
      case 18:
		// Inappropriate matching --> invalid qmail.schema
		$comment = "This error is caused by an invalid qmail.schema. Please look at README.OpenLDAP2 in your doc-directory.";
		break;
    }
    
    $msg = $errno . ": " . $error;
    
    if($comment){
		$msg .= " (" . $comment . ")";
    }
    
    return $msg;
}
// }}}

// {{{ pql_format_error_msg(ldap_linkid)
function pql_format_error_msg($ldap_linkid) {
	if($ldap_linkid)
	  echo pql_format_error_span("LDAP-Error: ") . ldap_error($ldap_linkid) . "<br>";
}
// }}}

// {{{ pql_format_error_span(msg)
function pql_format_error_span($msg) {
    return "<span class=\"error\">" . $msg . "</span>";
}
// }}}

// {{{ pql_format_status_msg(msg)
function pql_format_status_msg($msg){
?>
  <table cellpadding="3" cellspacing="0" border="0" width="100%">
    <th colspan="3" align="left">
      <tr class="message">
        <td colspan="2" class="message">
          <img src="images/info.png" width="16" height="16" border="0" align="left">
          <table>
            <?php echo stripslashes($msg) . "\n"; ?>
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

// {{{ pql_format_tree(text, url, links, level)
function pql_format_tree($text, $url, $links = '', $level = 0) {
	global $opera, $div_counter;

	if(empty($div_counter))
	  $div_counter = 0;

	$child  = $div_counter . "Child";
	$parent = "Parent";

	if($opera) {
		pql_format_tree_opera($div_counter, $text, $url, $level, $parent, $child);
	} else {
		pql_format_tree_other($div_counter, $text, $url, $level, $parent, $child);
	}

	if(is_array($links))
	  pql_format_tree_span($text, $links, $level);

	if($level)
	  pql_format_tree_end();

	$div_counter++;
}
// }}}

// {{{ pql_format_tree_opera(counter, text, url, level, parent, child)
function pql_format_tree_opera($cnt, $text, $url, $level, $parent, $child) {
	$nbsp = '';
	if($level) {
		for($i=0; $i < ($level * 4); $i++) 
		  $nbsp .= "&nbsp;";
	}
?>
  <div id="el<?=$cnt?><?=$parent?>" class="parent" onclick="showhide(el<?=$child?>, el<?=$cnt?>Img)">
    <?=$nbsp?><img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$cnt?>Img">
    <?php if($url) { ?><a href="<?=$url?>"><?php } ?><font color="black" class="heada"><?=$text?></font><?php if($url) { ?></a><?php } ?>

  </div>
  <span id="el<?=$child?>" style="display:''">
<?php	
}
// }}}

// {{{ pql_format_tree_other(counter, text, url, level, parent, child)
function pql_format_tree_other($cnt, $text, $url, $level, $parent, $child) {
	$nbsp = '';
	if($level) {
		for($i=0; $i < ($level * 4); $i++) 
		  $nbsp .= "&nbsp;";
	}

	if($url) {
		$url = "href=\"$url\"";
	}
?>
  <div id="el<?=$cnt?><?=$parent?>" class="parent">
    <a class="item" <?=$url?> onClick="if (capable) {expandBase('el<?=$cnt?>', true); return false;}">
      <?=$nbsp?><img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$cnt?>Img">
    <?php if($url) { ?></a><?php } ?>

    <?php if($url) { ?><a class="item" href="<?=$url?>"><?php } ?>

      <font color="black" class="heada"><?=$text?></font>
    <?php if($url) { ?></a><?php } ?>
  </div>
  <div id="el<?=$child?>" class="child">
<?php	
}
// }}}

// {{{ pql_format_tree_span(text, links, level)
function pql_format_tree_span($text, $links, $level) {
	$nbsp = '';
	for($i=0; $i < (($level + 1) * 4); $i++)
	  $nbsp .= "&nbsp;";
	
	foreach($links as $text => $link) {
		// If it's a user, we want an icon before the list...
		// DIRTY HACK: An option to the function instead?
		$icon = 0;
		if(eregi("user_detail\.php", $link))
		  $icon = 1;

		if($link) {
?>
    <nobr><?=$nbsp?><a href="<?=$link?>"><?php if($icon) { ?><img src="images/mail_small.png" border="0" alt="<?=$text?>">&nbsp;<?php } ?><?=$text?></a></nobr><br>
<?php	} else { ?>
    <nobr><?=$nbsp?><?php if($icon) { ?><img src="images/mail_small.png" border="0" alt="<?=$text?>">&nbsp;<?php } ?><?=$text?></nobr><br>
<?php	}
	}
}
// }}}

// {{{ pql_format_tree_end(void)
function pql_format_tree_end() {
	global $opera;

	if($opera)
	  echo "  </span>\n\n";
	else
	  echo "  </div>\n\n";
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
	$old = $string;

	$new = pql_format_international($old);
	if($old != $new) {
		// The two strings isn't equal -> something have been replaced

		if(($attrib != '') && ($linkid != '')) {
			// Retreive all EXISTING attributes the LDAP server knows about
			$ldap_attribs = pql_get_subschema($linkid, "attributetypes");
			
			// Find the syntax for the attribute (or it's superior attribute)
			if($ldap_attribs[$attrib]["SYNTAX"]) {
				$syntax = $ldap_attribs[$attrib]["SYNTAX"];
			} else {
				// No syntax, check the SUP if that have a SYNTAX
				// TODO: Be recursive!
				if($ldap_attribs[$attrib]["SUP"] && $ldap_attribs[$ldap_attribs[$attrib]["SUP"]]["SYNTAX"]) {
					$syntax = $ldap_attribs[$ldap_attribs[$attrib]["SUP"]]["SYNTAX"];
				}
			}
		
			// Does this attribute allow a UTF-8 encoded string?
			if(eregi('1\.3\.6\.1\.4\.1\.1466\.115\.121\.1\.15', $syntax))
			  return(utf8_encode($string));
			else
			  // Oups, no! Replace any national character with it's
			  // international (ie US char) counter part...
			  return($new);
		} else
		  return(utf8_encode($string));
	}

	return($string);
}

// }}}

// {{{ pql_maybe_decode(string)
// Check if we SHOULD decode the string...
function pql_maybe_decode($string) {
	if(eregi("\=$", $string))
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
			if(ereg("@", $old)) {
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
	if(eregi("xn--", $string) and function_exists("idn_to_utf8")) {
		// It's an IDNA string, and we have the IDN module loaded

		// Catch email addresses (can't decode the @)
		if(ereg("@", $string)) {
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
