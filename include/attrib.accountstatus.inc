<?php
// attribute plugin for
// accountstatus (status of mailaccount)
// $Id: attrib.accountstatus.inc,v 2.24 2005-01-28 12:34:50 turbo Exp $

/* DLW: When the calendar gets a value of 2003.12.25, it gets stored or displayed as 2003/12/24 (@7am)
 *      I think it's having problems with time zones.  Look into it. */

// {{{ attribute_check(type)
/* attribute_check is sometimes called directly (from attribute_print_form)
 * so I use _REQUEST variables instead of _POST variables. */
function attribute_check($type) {
    global $error, $LANG;
	global $date, $days;		// Output globals.
    
	if($type != "modify") {
		switch($_REQUEST["set"]) {
		  case "active":
		  case "nopop":
		  case "noaccess":
		  case "disabled":
		  case "expiration":
			return true;
			
		  default:
			$error["set"] = pql_complete_constant($LANG->_('Unknown attribute value %value% for %attribute%'),
												  array('value' => $set, 'attribute' => $LANG->_('accountstatus')));
			return false;
		}
	} else {
		$tmp = split("\.", $_REQUEST["date"]);

		// Convert the YYYY.MM.DD date to UNIX std time (sec from 1 Jan 1970)
		$date = date("U", mktime(0,0,0,$tmp[1],$tmp[2],$tmp[0]));

		// Get 'days from Jan 1, 1970' which is the correct integer for shadowExpire
		$days = split('\.', $date / (60 * 60 * 24));
		$days = $days[0];

		if($days) {
			return true;
		} else {
			return false;
		}
	}
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    // dummy function
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "set") {
    global $error, $LANG;

    if($_REQUEST["set"] != 'expiration') {
		// No form neccessary - save directly

		if(!attribute_check($type)) {
			$msg = $error["set"];
			attribute_forward($msg);
		}

		attribute_save($type);
	} else {
		// Output a form requesting date when account expires
		// Java script etc taken from http://dynarch.com/mishoo/calendar.epl
?>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3">
      <th colspan="3" align="left"><?=$LANG->_('Time when the account is disabled')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Expiration date')?></td>
          <td><input type="text" name="date" id="f_date_c" readonly="1" /></td>
          <td><img src="jscalendar/img.gif" id="f_trigger_c" style="cursor: pointer; border: 1px solid red;" title="Date selector"
                   onmouseover="this.style.background='red';" onmouseout="this.style.background=''" /></td>
        </tr>
      </th>
    </table>


    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

  <script type="text/javascript">
      Calendar.setup({
          inputField     :    "f_date_c",     // id of the input field
          ifFormat       :    "y.mm.dd",       // format of the input field (I'd prefere 'sec from 1970.01.01', but...)
          button         :    "f_trigger_c",  // trigger for the calendar (button ID)
          align          :    "Bl",           // alignment (Vertical: top, center, bottom. Horizontal: left, center, right)
          singleClick    :    true
      });
  </script>
<?php
	}
}
// }}}

// {{{ attribute_save(type)
/* attribute_save is sometimes called directly (from attribute_print_form)
 * so I use _REQUEST variables instead of _POST variables. */
function attribute_save($type) {
    global $_pql, $error, $LANG;
	global $date, $days;		// Set in attribute_check.

    switch($type) {
      case "fulldomain":
      case "modify":
		if($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_EXPIRATION")) {
			// Set the shadowExpire attribute for this user
			
			// Get the objectClasses of this user
			$entry = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], 'objectclass');

			// Make sure that the user have the 'shadowAccount' objectclass
			for($i=0; $entry[$i]; $i++) {
				$new_entry["objectclass"][] = $entry[$i];

				if($entry[$i] == pql_get_define("PQL_ATTR_EXPIRATION"))
				  $got_objectclass = 1;
			}

			// Add the objectclass if it doesn't already exists
			if(!$got_objectclass)
			  $new_entry["objectclass"][] = "shadowAccount";

			// Add the attribute and it's value
			$new_entry[pql_get_define("PQL_ATTR_EXPIRATION")] = $days;

			// Modify the object
			if(!pql_write_mod($_pql->ldap_linkid, $_REQUEST["user"], $new_entry, "attrib.accountstatus.inc:attribute_save()/1"))
			  $msg  = $LANG->_('Failed to set account expiration date') . ": " . pql_format_error(0);
			else
			  $msg = $LANG->_('Successfully set account expiration date');
		} else {
			// Create a user search filter (only look for mail users - !?!?).
			$filter  = "(&(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*)(";
			$filter .= pql_get_define("PQL_ATTR_MAIL")."=*))";
			
			// Retreive all users in this branch/domain.
			$users   = pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], $filter);
			if(is_array($users)) {
				foreach($users as $user)
				  if(!pql_modify_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_ISACTIVE"),
										   '', $_REQUEST["set"])) {
					  $msg  = pql_complete_constant($LANG->_('Failed to change %what%'),
													array('what' => $LANG->_('Status')));
					  $msg .= ": " . pql_format_error(0);
				  } else
					$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
												 array('what' => $LANG->_('Status'),
													   'new'  => pql_ldap_accountstatus($_REQUEST["set"])));
			}
		}

		attribute_forward($msg);
		break;
		
      case "set":
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_ISACTIVE"),
								1, $_REQUEST["set"])) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('Status'),
											   'new'  => pql_ldap_accountstatus($_REQUEST["set"])));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Status'))) . ": " . pql_format_error(0);
		
		if(file_exists("./.DEBUG_ME"))
		  die($msg);
		else
		  attribute_forward($msg);
		break;
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
