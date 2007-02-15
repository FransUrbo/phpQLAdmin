<?php
// attribute plugin for spamAssassin
// $Id: attrib.spamassassin.inc,v 2.7 2007-02-15 12:08:09 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;

    // is typed in ?
    if(empty($_REQUEST["where"])) {
	  $error["where"] = $LANG->_('Missing');
	  return false;
    }
    
    if(empty($_REQUEST["xspam"])) {
	  $error["xspam"] = $LANG->_('Missing');
	  return false;
    }
    
    if(empty($_REQUEST["what"])) {
	  $error["what"] = $LANG->_('Missing');
	  return false;
    }

	$_REQUEST["spamassassin"] = "add_header"." ".$_REQUEST["where"]." ".$_REQUEST["xspam"]." ".$_REQUEST["what"];
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
  $_REQUEST["spamassassin"] = $_REQUEST["oldvalue"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;

	$spamassassin = split(' ', $_REQUEST["spamassassin"]);

	unset($tmp);
	for($i=3; $spamassassin[$i]; $i++) {
	  $tmp .= $spamassassin[$i];
	  if($spamassassin[$i+1])
		$tmp .= " ";
	  unset($spamassassin[$i]);
	}
	$spamassassin[3] = $tmp;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for user %user%'), array('what' => $LANG->_('Mail header line'), 'user' => $_REQUEST["user"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Where')?></td>
          <td class="title"><input type="text" name="where" value="<?=$spamassassin[1]?>" size="50"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('X-Spam-???')?></td>
          <td class="title"><input type="text" name="xspam" value="<?=$spamassassin[2]?>" size="50"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('What')?></td>
          <td class="title"><input type="text" name="what" value="<?=$spamassassin[3]?>" size="50"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["spamassassin"]?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save($request)
function attribute_save($request) {
	global $_pql, $LANG;

	$entry  = $_REQUEST;
	$attrib = $_REQUEST["attrib"];

	// ------------------------------------
	// Remove some stuff that shouldn't be in the LDAP object
	unset($entry["rootdn"]); unset($entry["domain"]);
	unset($entry["user"]); unset($entry["PHPSESSID"]);
	unset($entry["commit"]); unset($entry["view"]);
	unset($entry["attrib"]); unset($entry["where"]);
	unset($entry["xspam"]); unset($entry["what"]);
	unset($entry["submit"]); unset($entry["oldvalue"]);

	// ------------------------------------
	// Convert the language array, to a flat variable
	if(is_array($_REQUEST["ok_languages"])) {
		unset($entry["ok_languages"]);

		foreach($_REQUEST["ok_languages"] as $lang)
		  $entry["ok_languages"] .= $lang . " ";
	}
	
	// ------------------------------------
	// Convert 'on' to '1'.
	if(@$_REQUEST["rewrite_subject"] == 'on')
	  $entry["rewrite_subject"] = 1;

	// ------------------------------------
	// If we haven't supplied BOTH of the values,
	// we don't use ANY of it.
	if(!$_REQUEST["spam_level_stars"] or !$entry["spam_level_char"]) {
		unset($entry["spam_level_stars"]);
		unset($entry["spam_level_char"]);
	}

	// ------------------------------------
	// Create a {white,black} listing variable
	if(is_array($_REQUEST["addresses"])) {
		unset($entry["addresses"]);

		for($i=0; $i < count($_REQUEST);) {
			$tag = $_REQUEST["addresses"][$i];

			$entry[$tag][] = $_REQUEST["addresses"][$i+1];
			$i = $i+2;
		}
	}

	// ------------------------------------
	// Just incase user didn't press 'Add new value',
	// add the last value that's in the input box.
	unset($entry["key"]); unset($entry["email"]);
	if($_REQUEST["key"] and $_REQUEST["email"]) {
		$tag = $_REQUEST["key"];
		$entry[$tag][] = $_REQUEST["email"];
	}

	// ------------------------------------
	// Convert this to an array that can be put in a
	// 'spamAssassin' attribute.
	if(is_array($entry)) {
	  foreach($entry as $key => $value) {
		if(is_array($value)) {
		  foreach($value as $list => $val)
			$new[$attrib][] = $key . " " . $val;
		} else
		  $new[$attrib][] = $key . " " . $value;
	  }
	}

	// ------------------------------------
	if($_REQUEST["attrib"] == 'add_header') {
	  // Get the old values from LDAP
	  $entry = $_pql->get_attribute(urldecode($_REQUEST["user"]),
								 pql_get_define("PQL_ATTR_SPAMASSASSIN"));

	  if($request == 'modify') {
		// Go through the old values, looking for the value to replace
		for($i=0; $entry[$i]; $i++) {
		  if($entry[$i] == $_REQUEST["oldvalue"])
			$entry[$i] = $_REQUEST["spamassassin"];
		}
	  } elseif($request == 'add') {
		$entry[] = $_REQUEST["spamassassin"];

		// Just to make it a little prettier...
		sort($entry);
	  }

	  // Replace the old value (_REQUEST[oldvalue]) with the
	  // new value (_REQUEST[spamassassin]).
	  if(pql_modify_attribute(urldecode($_REQUEST["user"]),
							  pql_get_define("PQL_ATTR_SPAMASSASSIN"), 1,
							  $entry))
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('Antispam configuration')));
	  else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('Antispam configuration')))
		  . ": " . ldap_error($_pql->ldap_linkid);
	} else {
	  // Update the user object, replacing the whole
	  // attribute set.
	  if(pql_modify_attribute(urldecode($_REQUEST["user"]), '', '', $new))
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('Antispam configuration')));
	  else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('Antispam configuration')))
		  . ": " . ldap_error($_pql->ldap_linkid);
	}

	attribute_forward($msg);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
