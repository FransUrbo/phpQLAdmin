<?php
// phpQLAdmin Application Programming Interface (API)
//
// Retreive LDAP connection and database status
// from the cn=Monitor ($_pql->ldap_monitor) backend.
//
// $Id: pql_status.inc,v 1.8 2007-02-12 15:40:28 turbo Exp $
//

// {{{ pql_get_status(linkid, dn, attribute)
function pql_get_status($linkid, $dn, $attrib) {
	$dn = pql_format_normalize_dn($dn);

	if(!is_array($attrib)) {
		$attrib = lc($attrib);
		$attribs = array($attrib);
	} else {
		foreach($attrib as $val) {
			$attribs[] = lc($val);
		}
	}

    $sr = @ldap_read($linkid, $dn, 'objectClass=*', $attribs);
    if($sr) {
		$entry = ldap_get_entries($linkid, $sr);
		if(!is_array($attrib) and $entry[0][$attrib]['count'] > 1) {
			for($i=0; $i < count($entry[0][$attrib]); $i++)
			  $new[] = $entry[0][$attrib][$i];

			return($new);
		} elseif($entry[0]['count'] <= 1) {
			return($entry[0][$attrib][0]);
		} else {
			// Return an array - we got more than one value
			foreach($attribs as $attrib) {
			  for($i=0; $i < $entry[0][$attrib]['count']; $i++) {
				if($entry[0][$attrib]['count'] > 1)
				  $values[$attrib][] = $entry[0][$attrib][$i];
				else
				  $values[$attrib] = $entry[0][$attrib][$i];
			  }
			}
			
			if(!empty($values))
			  return($values);
			else
			  return false;
		}
	}

	return(NULL);
}
// }}}

// {{{ pql_get_subentries(linkid, basedn, attribute, filter)
// If called without attrib and filter -> just return number
// of entries found.
function pql_get_subentries($linkid, $basedn, $attrib = 0, $filter = 0) {
	$basedn = pql_format_normalize_dn($basedn);

	if(!$attrib and !$filter) {
		$sr = @ldap_list($linkid, $basedn, 'objectClass=*');
		if($sr) {
			$entry = ldap_get_entries($linkid, $sr);
			
			if($entry['count'])
			  return($entry['count']);
		}
	} else {
		$attrib = lc($attrib);

		$sr = @ldap_list($linkid, $basedn, $filter, array($attrib));
		if($sr) {
			$entry = ldap_get_entries($linkid, $sr);

			for($i=0; $i < $entry['count']; $i++) {
				$values[] = $entry[$i][$attrib][0];
			}

			return($values);
		}
	}

	return(NULL);
}
// }}}

// {{{ pql_get_status_databases(suffix)
function pql_get_status_databases($suffix = 0) {
	global $_pql;
	$filter  = '(&(cn=Database*)(|(namingContexts=*)(monitorContext=*)))';
	$attribs = array('monitoredInfo', 'namingContexts', 'monitorContext', 'seeAlso');

	$sr1 = ldap_list($_pql->ldap_linkid, 'cn=Databases,'.$_pql->ldap_monitor, $filter, $attribs);
	if($sr1) {
		$entry1 = ldap_get_entries($_pql->ldap_linkid, $sr1);

		for($i=0; $i < $entry1['count']; $i++) {
			foreach($attribs as $attrib) {
				$attrib = lc($attrib);
				
				if(($attrib == 'seealso') and $entry1[$i]['seealso'][0]) {
					// Follow reference to backend information
					$sr2 = ldap_read($_pql->ldap_linkid, pql_format_normalize_dn($entry1[$i]['seealso'][0]),
									 'objectClass=*', array('supportedControl'));
					if($sr2) {
						$entry2 = ldap_get_entries($_pql->ldap_linkid, $sr2);
						
						for($j=0; $j < $entry2[0]['supportedcontrol']['count']; $j++) {
							$values[$entry1[$i]['dn']]['supportedcontrol'][] = $entry2[0]['supportedcontrol'][$j];
						}
					}
				} elseif(@$entry1[$i][$attrib][0] == $suffix)
				  return(array($attrib => $suffix, 'monitoredinfo' => $entry1[$i]['monitoredinfo'][0]));
				elseif(@$entry1[$i][$attrib][0])
				  $values[$entry1[$i]['dn']][$attrib] = $entry1[$i][$attrib][0];
			}
		}

		return($values);
	}

	return(NULL);
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
