<?php
// Get the ACIs in this branch object
$aci = pql_aci_get($_pql->ldap_linkid, urldecode($domain));

echo "These are the ACIs that's configured for this object. Not that I know what I would like to do with them, but anyway...";
printr($aci);
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
