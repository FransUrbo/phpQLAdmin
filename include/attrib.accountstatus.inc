<?php
// attribute plugin for
// accountstatus (status of mailaccount)
// $Id: attrib.accountstatus.inc,v 2.15 2004-01-01 12:35:33 turbo Exp $

function attribute_check($type) {
    global $error, $view, $set, $domain, $rootdn, $attrib, $user, $date, $days, $LANG, $_pql;
    
	if($type != "modify") {
		switch($set) {
		  case "active":
		  case "nopop":
		  case "disabled":
		  case "expiration":
			return true;
			
		  default:
			$error["set"] = pql_complete_constant($LANG->_('Unknown attribute value %value% for %attribute%'),
												  array('value' => $set, 'attribute' => $LANG->_('accountstatus')));
			return false;
		}
	} else {
		$tmp = split("\.", $date);

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

function attribute_init() {
    // dummy function
}

function attribute_print_form($type = "set") {
    global $error, $view, $set, $domain, $rootdn, $attrib, $user, $LANG, $_pql;

    if($set != 'expiration') {
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

  <form action="<?=$PHP_SLEF?>" method="post">
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
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="view"     value="<?=$view?>">
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

function attribute_save($type) {
    global $rootdn, $domain, $user, $set, $attrib, $_pql, $view, $days, $LANG;
    
    switch($type) {
      case "fulldomain":
      case "modify":
		if(eregi(pql_get_define("PQL_GLOB_ATTR_EXPIRATION"), $attrib)) {
			// Set the shadowExpire attribute for this user
			
			// Get the objectClasses of this user
			$entry = pql_get_attribute($_pql->ldap_linkid, $user, 'objectclass');

			// Make sure that the user have the 'shadowAccount' objectclass
			for($i=0; $entry[$i]; $i++) {
				$new_entry["objectclass"][] = $entry[$i];

				if(eregi(pql_get_define("PQL_GLOB_ATTR_EXPIRATION"), $entry[$i]))
				  $got_objectclass = 1;
			}

			// Add the objectclass if it doesn't already exists
			if(!$got_objectclass)
			  $new_entry["objectclass"][] = "shadowAccount";

			// Add the attribute and it's value
			$new_entry[pql_get_define("PQL_GLOB_ATTR_EXPIRATION")] = $days;

			// Modify the object
			if(!@ldap_mod_replace($_pql->ldap_linkid, $user, $new_entry)) {
				// Failed to REPLACE the value. Try to remove and then add it again
				unset($new_entry);

				$new_entry[pql_get_define("PQL_GLOB_ATTR_EXPIRATION")] = array();
				if(@ldap_mod_del($_pql->ldap_linkid, $user, $new_entry)) {
					$new_entry[pql_get_define("PQL_GLOB_ATTR_EXPIRATION")] = $days;
					if(@ldap_mod_add($_pql->ldap_linkid, $user, $new_entry)) {
						$msg = $LANG->_('Successfully set account expiration date');
					}
				}

				if(!$msg) {
					$msg  = $LANG->_('Failed to set account expiration date');
					$msg .= ": " . pql_format_error(0);
				}
			} else {
				$msg = $LANG->_('Successfully set account expiration date');
			}
		} else {
			$users = pql_user_get($_pql->ldap_linkid, $domain);
			
			if(is_array($users)) {
				foreach($users as $user)
				  if(! pql_replace_attribute($_pql->ldap_linkid, $user,
												 pql_get_define("PQL_GLOB_ATTR_ISACTIVE"),
												 $set)) {
					  $msg  = pql_complete_constant($LANG->_('Failed to change %what%'),
													array('what' => $LANG->_('Status')));
					  $msg .= ": " . pql_format_error(0);
				  } else
					$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
												 array('what' => $LANG->_('Status'),
													   'new'  => pql_ldap_accountstatus($set)));
			}
		}

		attribute_forward($msg);
		break;
		
      case "set":
		if(pql_replace_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_ISACTIVE"), $set)) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('Status'),
											   'new'  => pql_ldap_accountstatus($set)));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Status'))) . ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
