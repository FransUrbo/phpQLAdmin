<?php
// ----------------------------
// pql_formating.inc
// Output formating etc
//
// $Id: pql_formating.inc,v 2.23.10.1.4.1 2004-10-05 07:05:46 turbo Exp $

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
    return "<span class=\"error\">" . $msg . "</span>\n";
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
	global $div_counter;

	if(empty($div_counter))
	  $div_counter = 0;

	$child  = $div_counter . "Child";
	$parent = "Parent";

	if($_SESSION["opera"]) {
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

	$mouse["click"] = 'onclick="showhide(el'.$child.', el'.$cnt.'Img)"';
	$mouse["over"]  = 'onmouseover="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#CCFFCC\')}"';
	$mouse["out"]   = 'onmouseout="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#D0DCE0\')}"';

	if($level) {
		for($i=0; $i < ($level * 4); $i++) 
		  $nbsp .= "&nbsp;";
	}
?>
  <div id="el<?=$cnt?><?=$parent?>" class="parent" <?=$mouse["over"]." ".$mouse["out"]." ".$mouse["click"]?>>
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

	$mouse["over"]  = 'onmouseover="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#CCFFCC\')}"';
	$mouse["out"]   = 'onmouseout="if (isDOM || isIE4) {hilightBase(\'el'.$cnt.'\', \'#D0DCE0\')}"';

	if($level) {
		for($i=0; $i < ($level * 4); $i++) 
		  $nbsp .= "&nbsp;";
	}

	if($url) {
		if(eregi('^http://', $url))
		  $url = "href=\"$url\" target=\"_top\"";
		else
		  $url = "href=\"$url\"";
	}
?>
  <div id="el<?=$cnt?>Parent" class="parent" <?=$mouse["over"]." ".$mouse["out"]?>>
    <nobr>
      <a class="item" <?=$url?> onclick="if (capable) {expandBase('el<?=$cnt?>', true); return false;}">
        <?=$nbsp?><img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$cnt?>Img">
      </a>

      <a class="item" <?=$url?> onclick="if (capable) {expandBase('el<?=$cnt?>', false)}">
        <span class="heada"><?=$text?></span>
      </a>
    </nobr>
  </div>

  <div id="el<?=$child?>" class="child" style="margin-bottom: 5px" <?=$mouse["over"]." ".$mouse["out"]?>>
<?php	
}
// }}}

// {{{ pql_format_tree_span(text, links, level)
function pql_format_tree_span($text, $links, $level) {
	$nbsp = '';
	for($i=0; $i < (($level + 1) * 4); $i++)
	  $nbsp .= "&nbsp;";

	$count = count($links); $counter = 0;
	foreach($links as $text => $link) {
		// If it's a user, we want an icon before the list...
		// DIRTY HACK: An option to the function instead?
		$icon = 0;
		if(eregi("user_detail\.php", $link))
		  $icon = 1;

		if($text) {
			if($link) {
?>
    <nobr>
      <?=$nbsp?>
      <?php if($icon) { ?><img src="images/mail_small.png" border="0" alt="<?=$text?>"><?php } ?>
      <a class="tblItem" title="<?=$text?>" href="<?=$link?>" <?php if(eregi('^http://', $link)) { echo 'target="_new"'; } else { echo 'target="pqlmain"'; } ?>><?=$text?></a>
    </nobr>
<?php			if($counter < ($count - 1)) { ?>

    <br />
<?php			}
			} else { ?>
    <nobr>
      <?=$nbsp?>
      <?php if($icon) { ?><img src="images/mail_small.png" border="0" alt="<?=$text?>"><?php } ?>
      <?=$text?>
    </nobr>

    <br />
<?php		}
		} else { ?>
    <nobr>
      <?=$nbsp?>
      ---------------
    </nobr>
<?php		if($counter < ($count - 1)) { ?>

    <br />
<?php		}
		}

		$counter++;
	}
}
// }}}

// {{{ pql_format_tree_end(void)
function pql_format_tree_end() {
	if($_SESSION["opera"])
	  echo "  </span>\n\n";
	else
	  echo "  </div>\n\n";
}
// }}}

// {{{ pql_format_timestamp_human(beg, end)
function pql_format_timestamp_human($beg, $end) {
    global $LANG;

    $seconds         = $end - $beg;

	// -------------------------
	// Get number of DAYS of uptime
    $tmp  = $seconds / 86400;
	if(ereg('\.', $tmp)) {
		$tmp = split('\.', $tmp);
		$uptime['days'] = $tmp[0];
	} else
	  $uptime['days'] = $tmp;

	$uptime['weeks'] = round($uptime['days'] / 7);
	$uptime['years'] = round($uptime['weeks'] / 52);

	// -------------------------
	// Get number of HOURS of uptime
	$tmp = $seconds / 3600;
	if(ereg('\.', $tmp)) {
		$tmp = split('\.', $tmp);
		$uptime['hour'] = $tmp[0];
	} else
	  $uptime['hour'] = $tmp;

	// Take into account that we might have days as well.
	// Othervise we'll get '1 day, 24 hours, -1374 minutes'
	// when it's actually '1 day, 7 minutes'
	$uptime['hour'] = $uptime['hour'] - ($uptime['days'] * 24);

	// -------------------------
	// Get number of MINUTES of uptime
    $uptime['min']   = round( ($seconds - (($uptime['days'] * 86400) + ($uptime['hour'] * 3600))) / 60);
	if($uptime['min'] < 0) {
		$uptime['min']--;
		$uptime['min'] = 60 + $uptime['min'];
	}

	// -------------------------
	// Get number of SECONDS of uptime
    $uptime['sec']   = $seconds - (($uptime['days'] * 86400) + ($uptime['hour'] * 3600) + ($uptime['min'] * 60));
	if($uptime['sec'] < 0) {
		$uptime['min']--;
		$uptime['sec']--;

		$uptime['sec'] = 60 + $uptime['sec'];
	}
    
	// -------------------------
	// Prettify the uptime
	$uptime_string['years']   = ($uptime['years']) ? $uptime['years']." ".$LANG->_('years').", " : "";
	$uptime_string['weeks']   = ($uptime['weeks']) ? $uptime['weeks']." ".$LANG->_('weeks').", " : "";
	$uptime_string['days']    = ($uptime['days'])  ? $uptime['days']." ".$LANG->_('days').", "   : "";
	$uptime_string['hours']   = ($uptime['hour'])  ? $uptime['hour']." ".$LANG->_('hours').", " : "";
	$uptime_string['minutes'] = ($uptime['min'])   ? $uptime['min']." ".$LANG->_('minutes').", " : "";
	$uptime_string['seconds'] = ($uptime['sec'])   ? $uptime['sec']." ".$LANG->_('seconds')."." : "";

	$string  = $uptime_string['years'].$uptime_string['weeks'].$uptime_string['days'];
	$string .= $uptime_string['hours'].$uptime_string['minutes'].$uptime_string['seconds']."\n";
	return($string);
}
// }}}

// {{{ pql_format_timestamp_unixtime(timestamp)
function pql_format_timestamp_unixtime($timestamp) {
	$timestamp = preg_replace('/Z$/', '', $timestamp);

	$year   = $timestamp[0].$timestamp[1].$timestamp[2].$timestamp[3];
	$month  = $timestamp[4].$timestamp[5];
	$day    = $timestamp[6].$timestamp[7];

	$hour   = $timestamp[8].$timestamp[9];
	$minute = $timestamp[10].$timestamp[11];
	$second = $timestamp[12].$timestamp[13];

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
	if(ereg('%3D', $string)) {
		// The original string should be URL decoded
		$string = urldecode($string);
	}
	$tmp = $string;

	// Make sure we can have a ' in branch (also affects the user DN).
	if(ereg("\'", $tmp)) {
		if(ereg("\\\'", $tmp)) {
			// The string already contains \', remove it
			$tmp = ereg_replace("\\\'", "'", $tmp);
		} else {
			$tmp = ereg_replace("\'", "'", $tmp);
		}

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
	if($value == 'TRUE') {
		settype($value, "boolean");
		$value = '1';
	} elseif($value == 'FALSE') {
		settype($value, "boolean");
		$value = '0';
	}

	return($value);
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
			// 1.3.6.1.4.1.1466.115.121.1.15	UTF-8 encoded string
			// 1.3.6.1.4.1.1466.115.121.1.41	? - not listed...
			//		Only attributes that uses .41 are:
			//		postalAddress, registeredAddress and homePostalAddress
			//		-> I hope that it's ok to UTF-8 encode these values.
			//		   The database (OpenLDAP 2.0) accepts it anyway.
			if(eregi('1\.3\.6\.1\.4\.1\.1466\.115\.121\.1\.15', $syntax) or
			   eregi('1\.3\.6\.1\.4\.1\.1466\.115\.121\.1\.41', $syntax))
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
