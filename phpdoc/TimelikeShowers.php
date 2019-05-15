<html>
<head>
<title>Timelike Showers</title>
<link rel="stylesheet" type="text/css" href="pythia.css"/>
<link rel="shortcut icon" href="pythia32.gif"/>
</head>
<body>

<script language=javascript type=text/javascript>
function stopRKey(evt) {
var evt = (evt) ? evt : ((event) ? event : null);
var node = (evt.target) ? evt.target :((evt.srcElement) ? evt.srcElement : null);
if ((evt.keyCode == 13) && (node.type=="text"))
{return false;}
}

document.onkeypress = stopRKey;
</script>
<?php
if($_POST['saved'] == 1) {
if($_POST['filepath'] != "files/") {
echo "<font color='red'>SETTINGS SAVED TO FILE</font><br/><br/>"; }
else {
echo "<font color='red'>NO FILE SELECTED YET.. PLEASE DO SO </font><a href='SaveSettings.php'>HERE</a><br/><br/>"; }
}
?>

<form method='post' action='TimelikeShowers.php'>
    
<h2>Timelike Showers</h2>

The PYTHIA algorithm for timelike final-state showers is based on
the recent article [<a href="Bibliography.php" target="page">Sjo05</a>], where a transverse-momentum-ordered
evolution scheme is introduced. This algorithm is influenced by
the previous mass-ordered algorithm in PYTHIA [<a href="Bibliography.php" target="page">Ben87</a>] and by 
the dipole-emission formulation in Ariadne [<a href="Bibliography.php" target="page">Gus86</a>]. From the 
mass-ordered algorithm it inherits a merging procedure for first-order 
gluon-emission matrix elements in essentially all two-body decays 
in the standard model and its minimal supersymmetric extension 
[<a href="Bibliography.php" target="page">Nor01</a>]. 

<p/>
The normal user is not expected to call <code>TimeShower</code> directly, 
but only have it called from <code>Pythia</code>. Some of the parameters 
below, in particular <code>TimeShower:alphaSvalue</code>, would be of 
interest for a tuning exercise, however. 

<h3>Main variables</h3>

Often the maximum scale of the FSR shower evolution is understood from the
context. For instance, in a resonace decay half the resonance mass sets an
absolute upper limit. For a hard process in a hadronic collision the choice
is not as unique. Here the factorization scale has been chosen as the 
maximum evolution scale. This would be the <i>pT</i> for a 
<i>2 -> 2</i> process, supplemented by mass terms for massive outgoing 
particles. Some small amount of freedom is offered by
<br/><br/><table><tr><td><strong>TimeShower:pTmaxFudge </td><td></td><td> <input type="text" name="1" value="1.0" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>1.0</strong></code>; <code>minimum = 0.5</code>; <code>maximum = 2.0</code>)</td></tr></table>
While the above rules would imply that <i>pT_max = pT_factorization</i>, 
<code>pTmaxFudge</code> introduced a multiplicative factor <i>f</i> such 
that instead <i>pT_max = f * pT_factorization</i>. Only applies to the 
hardest interaction in an event. It is strongly suggested that 
<i>f = 1</i>, but variations around this default can be useful to test 
this assumption. 
  

<p/>
The amount of QCD radiation in the shower is determined by 
<br/><br/><table><tr><td><strong>TimeShower:alphaSvalue </td><td></td><td> <input type="text" name="2" value="0.137" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.137</strong></code>; <code>minimum = 0.06</code>; <code>maximum = 0.25</code>)</td></tr></table>
The <i>alpha_strong</i> value at scale <i>M_Z^2</i>. The default 
value corresponds to a crude tuning to LEP data, to be improved.
  

<p/>
The actual value is then regulated by the running to the scale 
<i>pT^2</i>, at which the shower evaluates <i>alpha_strong</i>

<br/><br/><table><tr><td><strong>TimeShower:alphaSorder  </td><td>  &nbsp;&nbsp;(<code>default = <strong>1</strong></code>; <code>minimum = 0</code>; <code>maximum = 2</code>)</td></tr></table>
Order at which <ei>alpha_strong</ei> runs,
<br/>
<input type="radio" name="3" value="0"><strong>0 </strong>: zeroth order, i.e. <ei>alpha_strong</ei> is kept  fixed.<br/>
<input type="radio" name="3" value="1" checked="checked"><strong>1 </strong>: first order, which is the normal value.<br/>
<input type="radio" name="3" value="2"><strong>2 </strong>: second order. Since other parts of the code do  not go to second order there is no strong reason to use this option,  but there is also nothing wrong with it.<br/>

<p/>
QED radiation is regulated by the <i>alpha_electromagnetic</i>
value at the <i>pT^2</i> scale of a branching.
 
<br/><br/><table><tr><td><strong>TimeShower:alphaEMorder  </td><td>  &nbsp;&nbsp;(<code>default = <strong>1</strong></code>; <code>minimum = -1</code>; <code>maximum = 1</code>)</td></tr></table>
The running of <ei>alpha_em</ei>.
<br/>
<input type="radio" name="4" value="1" checked="checked"><strong>1 </strong>: first-order running, constrained to agree with <code>StandardModel:alphaEMmZ</code> at the <ei>Z^0</ei> mass. <br/>
<input type="radio" name="4" value="0"><strong>0 </strong>: zeroth order, i.e. <ei>alpha_em</ei> is kept  fixed at its value at vanishing momentum transfer.<br/>
<input type="radio" name="4" value="-1"><strong>-1 </strong>: zeroth order, i.e. <ei>alpha_em</ei> is kept  fixed, but at <code>StandardModel:alphaEMmZ</code>, i.e. its value at the <ei>Z^0</ei> mass. <br/>

<p/>
The rate of radiation if divergent in the <i>pT -> 0</i> limit. Here, 
however, perturbation theory is expected to break down. Therefore an 
effective <i>pT_min</i> cutoff parameter is introduced, below which 
no emissions are allowed. The cutoff may be different for QCD and QED 
radiation off quarks, and is mainly a technical parameter for QED 
radiation off leptons.

<br/><br/><table><tr><td><strong>TimeShower:pTmin </td><td></td><td> <input type="text" name="5" value="0.5" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.5</strong></code>; <code>minimum = 0.1</code>; <code>maximum = 2.0</code>)</td></tr></table>
Parton shower cut-off <i>pT</i> for QCD emissions.
  

<br/><br/><table><tr><td><strong>TimeShower:pTminChgQ </td><td></td><td> <input type="text" name="6" value="0.5" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.5</strong></code>; <code>minimum = 0.1</code>; <code>maximum = 2.0</code>)</td></tr></table>
Parton shower cut-off <i>pT</i> for photon coupling to coloured particle.
  

<br/><br/><table><tr><td><strong>TimeShower:pTminChgL </td><td></td><td> <input type="text" name="7" value="0.0005" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.0005</strong></code>; <code>minimum = 0.0001</code>; <code>maximum = 2.0</code>)</td></tr></table>
Parton shower cut-off <i>pT</i> for pure QED branchings. 
Assumed smaller than (or equal to) <code>pTminChgQ</code>.
  

<p/> 
Shower branchings <i>gamma -> f fbar</i>, where <i>f</i> is a 
quark or lepton, in part compete with the hard processes involving 
<i>gamma^*/Z^0</i> production. In order to avoid overlap it makes
sense to correlate the maximum <i>gamma</i> mass allowed in showers
with the minumum <i>gamma^*/Z^0</i> mass allowed in hard processes.
In addition, the shower contribution only contains the pure 
<i>gamma^*</i> contribution, i.e. not the <i>Z^0</i> part, so
the mass spectrum above 50 GeV or so would not be well described.

<br/><br/><table><tr><td><strong>TimeShower:mMaxGamma </td><td></td><td> <input type="text" name="8" value="10.0" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>10.0</strong></code>; <code>minimum = 0.001</code>; <code>maximum = 50.0</code>)</td></tr></table>
Maximum invariant mass allowed for the created fermion pair in a 
<i>gamma -> f fbar</i> branching in the shower.
  

<h3>Interleaved evolution</h3>

Multiple interactions (MI) and initial-state showers (ISR) are 
always interleaved, as follows. Starting from the hard interaction, 
the complete event is constructed by a set of steps. In each step 
the <i>pT</i> scale of the previous step is used as starting scale 
for a downwards evolution. The MI and ISR components each make
their respective Monte Carlo choices for the next lower <i>pT</i> 
value. The one with larger <i>pT</i> is allowed to carry out its 
proposed action, thereby modifying the conditions for the next steps. 
This is relevant since the two components compete for the energy 
contained in the beam remnants: both an interaction and an emission 
take avay some of the energy, leaving less for the future. The end 
result is a combined chain of decreasing <i>pT</i> values, where 
ones associated with new interactions and ones with new emissions 
are interleaved.  

<p/>
There is no corresponding requirement for final-state radiation (FSR)
to be interleaved. Such an FSR emission does not compete directly for 
beam energy (but see below), and also can be viewed as occuring after 
the other two components in some kind of time sense. Interleaving is 
allowed, however, since it can be argued that a high-<i>pT</i> FSR 
occurs on shorter time scales than a low-<i>pT</i> MI, say. 
Backwards evolution of ISR is also an example that physical time 
is not the only possible ordering principle, but that one can work 
with conditional probabilities: given the partonic picture at a  
specific <i>pT</i> resolution scale, what possibilities are open 
for a modified picture at a slightly lower <i>pT</i> scale, either 
by MI, ISR or FSR? Complete interleaving of the three components also 
offers advantages if one aims at matching to higher-order matrix 
elements above some given scale.

<br/><br/><strong>TimeShower:interleave</strong>  <input type="radio" name="9" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="9" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
If on, final-state emissions are interleaved in the same 
decreasing-<i>pT</i> chain as multiple interactions and initial-state
emissions. If off, final-state emissions are only addressed after the
multiple interactions and initial-state radiation have been considered.
  

<p/>
As an aside, it should be noted that such interleaving does not affect 
showering in resonance decays, such as a <i>Z^0</i>. These decays are 
only introduced after the production process has been considered in full, 
and the subsequent FSR is carried out inside the resonance, with 
preserved resonance mass.

<p/>
One aspect of FSR for a hard process in hadron collisions is that often
colour diples are formed between a scattered parton and a beam remnant,
or rather the hole left behind by an incoming partons. If such holes
are allowed as dipole ends and take the recoil when the scattered parton 
undergoes a branching then this translates into the need to take some
amount of remnant energy also in the case of FSR, i.e. the roles of 
ISR and FSR are not completely decoupled. The energy taken away is
bokkept by increasing the <i>x</i> value assigned to the incoming 
scattering parton, and a reweighting factor 
<i>x_new f(x_new, pT^2) / x_old f(x_old, pT^2)</i> 
in the emission probability ensures that not unphysically large 
<i>x_new</i> values are reached. Usually such <i>x</i> changes are 
small, and they can be viewed as a higher-order effect beyond the
accuracy of the leading-log initial-state showers. 

<p/>
This choice is not unique, however. As an alternative, if nothing else
useful for cross-checks, one could imagine that the FSR is completely
decoupled from the ISR and beam remnants. 

<br/><br/><strong>TimeShower:allowBeamRecoil</strong>  <input type="radio" name="10" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="10" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
If on, the final-state shower is allowed to borrow energy from 
the beam remnants as described above, thereby changing the mass of the 
scattering subsystem. If off, the partons in the scattering subsystem 
are constrained to borrow energy from each other, such that the total
four-momentum of the system is preserved. This flag has no effect 
on resonance decays, where the shower always preserves the resonance 
mass, cf. the comment above about showers for resonances never being 
interleaved. 
  

 
<h3>Radiation off octet onium states</h3>

In the current implementation, charmonium and bottomonium production
can proceed either through colour singlet or colour octet mechanisms,
both of them implemented in terms of <i>2 -> 2</i> hard processes
such as <i>g g -> (onium) g</i>.
In the former case the state does not radiate and the onium therefore 
is produced in isolation, up to normal underlying-event activity. In 
the latter case the situation is not so clear, but it is sensible to 
assume that a shower can evolve. (Assuming, of course, that the 
transverse momentum of the onium state is sufficiently high that  
radiation is of relevance.)

<p/> 
There could be two parts to such a shower. Firstly a gluon (or even a 
quark, though less likely) produced in a hard <i>2 -> 2</i> process 
can undergo showering into many gluons, whereof one branches into the 
heavy-quark pair. Secondly, once the pair has been produced, each quark 
can radiate further gluons. This latter kind of emission could easily 
break up a semibound quark pair, but might also create a new semibound 
state where before an unbound pair existed, and to some approximation
these two effects should balance in the onium production rate. 
The showering "off an onium state" as implemented here therefore should 
not be viewed as an accurate description of the emission history
step by step, but rather as an effective approach to ensure that the 
octet onium produced "in the hard process" is embedded in a realistic 
amount of jet activity. 
Of course both the isolated singlet and embedded octet are likely to
be extremes, but hopefully the mix of the two will strike a reasonable 
balance. However, it is possible that some part of the octet production 
occurs in channels where it should not be accompanied by (hard) radiation. 
Therefore reducing the fraction of octet onium states allowed to radiate 
is a valid variation to explore uncertainties. 

<p/>
If an octet onium state is chosen to radiate, the simulation of branchings 
is based on the assumption that the full radiation is provided by an 
incoherent sum of radiation off the quark and off the antiquark of the 
onium state. Thus the splitting kernel is taken to be the normal 
<i>q -> q g</i> one, multiplied by a factor of two. Obviously this is 
a simplification of a more complex picture, averaging over factors pulling
in different directions. Firstly, radiation off a gluon ought
to be enhanced by a factor 9/4 relative to a quark rather than the 2
now used, but this is a minor difference. Secondly, our use of the 
<i>q -> q g</i> branching kernel is roughly equivalent to always
following the harder gluon in a <i>g -> g g</i> branching. This could 
give us a bias towards producing too hard onia. A soft gluon would have 
little phase space to branch into a heavy-quark pair however, so the
bias may not be as big as it would seem at first glance. Thirdly, 
once the gluon has branched into a quark pair, each quark carries roughly 
only half of the onium energy. The maximum energy per emitted gluon should 
then be roughly half the onium energy rather than the full, as it is now. 
Thereby the energy of radiated gluons is exaggerated, i.e. onia become too 
soft. So the second and the third points tend to cancel each other. 

<p/>
Finally, note that the lower cutoff scale of the shower evolution depends 
on the onium mass rather than on the quark mass, as it should be. Gluons
below the octet-onium scale should only be part of the octet-to-singlet 
transition.

<br/><br/><table><tr><td><strong>TimeShower:octetOniumFraction </td><td></td><td> <input type="text" name="11" value="1." size="20"/>  &nbsp;&nbsp;(<code>default = <strong>1.</strong></code>; <code>minimum = 0.</code>; <code>maximum = 1.</code>)</td></tr></table>
Allow colour-octet charmonium and bottomonium states to radiate gluons.
0 means that no octet-onium states radiate, 1 that all do, with possibility
to interpolate between these two extremes. 
  

<br/><br/><table><tr><td><strong>TimeShower:octetOniumColFac </td><td></td><td> <input type="text" name="12" value="2." size="20"/>  &nbsp;&nbsp;(<code>default = <strong>2.</strong></code>; <code>minimum = 0.</code>; <code>maximum = 4.</code>)</td></tr></table>
The colour factor used used in the splitting kernel for those octet onium 
states that are allowed to radiate, normalized to the <i>q -> q g</i>
splitting kernel. Thus the default corresponds to twice the radiation
off a quark. The physically preferred range would be between 1 and 9/4.
  

<h3>Further variables</h3>

There are several possibilities you can use to switch on or off selected
branching types in the shower, or in other respects simplify the shower.
These should normally not be touched. Their main function is for 
cross-checks.

<br/><br/><strong>TimeShower:QCDshower</strong>  <input type="radio" name="13" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="13" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
Allow a QCD shower, i.e. branchings <i>q -> q g</i>, <i>g -> g g</i> 
and <i>g -> q qbar</i>; on/off = true/false.
  

<br/><br/><table><tr><td><strong>TimeShower:nGluonToQuark  </td><td></td><td> <input type="text" name="14" value="5" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>5</strong></code>; <code>minimum = 0</code>; <code>maximum = 5</code>)</td></tr></table>
Number of allowed quark flavours in <i>g -> q qbar</i> branchings
(phase space permitting). A change to 4 would exclude 
<i>g -> b bbar</i>, etc. 
  

<br/><br/><strong>TimeShower:QEDshowerByQ</strong>  <input type="radio" name="15" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="15" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
Allow quarks to radiate photons, i.e. branchings <i>q -> q gamma</i>; 
on/off = true/false.
  

<br/><br/><strong>TimeShower:QEDshowerByL</strong>  <input type="radio" name="16" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="16" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
Allow leptons to radiate photons, i.e. branchings <i>l -> l gamma</i>;  
on/off = true/false.
  

<br/><br/><strong>TimeShower:QEDshowerByGamma</strong>  <input type="radio" name="17" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="17" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
Allow photons to branch into lepton or quark pairs, i.e. branchings 
<i>gamma -> l+ l-</i> and <i>gamma -> q qbar</i>;
on/off = true/false.
  

<br/><br/><table><tr><td><strong>TimeShower:nGammaToQuark  </td><td></td><td> <input type="text" name="18" value="5" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>5</strong></code>; <code>minimum = 0</code>; <code>maximum = 5</code>)</td></tr></table>
Number of allowed quark flavours in <i>gamma -> q qbar</i> branchings
(phase space permitting). A change to 4 would exclude 
<i>g -> b bbar</i>, etc. 
  

<br/><br/><table><tr><td><strong>TimeShower:nGammaToLepton  </td><td></td><td> <input type="text" name="19" value="3" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>3</strong></code>; <code>minimum = 0</code>; <code>maximum = 3</code>)</td></tr></table>
Number of allowed lepton flavours in <i>gamma -> l+ l-</i> branchings
(phase space permitting). A change to 2 would exclude 
<i>gamma -> tau+ tau-</i>, and a change to 1 also 
<i>gamma -> mu+ mu-</i>. 
  

<br/><br/><strong>TimeShower:MEcorrections</strong>  <input type="radio" name="20" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="20" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
Use of matrix element corrections where available; on/off = true/false.
  

<br/><br/><strong>TimeShower:phiPolAsym</strong>  <input type="radio" name="21" value="on" checked="checked"><strong>On</strong>
<input type="radio" name="21" value="off"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>on</strong></code>)<br/>
Azimuthal asymmetry induced by gluon polarization; on/off = true/false.
  

<input type="hidden" name="saved" value="1"/>

<?php
echo "<input type='hidden' name='filepath' value='".$_GET["filepath"]."'/>"?>

<table width="100%"><tr><td align="right"><input type="submit" value="Save Settings" /></td></tr></table>
</form>

<?php

if($_POST["saved"] == 1)
{
$filepath = $_POST["filepath"];
$handle = fopen($filepath, 'a');

if($_POST["1"] != "1.0")
{
$data = "TimeShower:pTmaxFudge = ".$_POST["1"]."\n";
fwrite($handle,$data);
}
if($_POST["2"] != "0.137")
{
$data = "TimeShower:alphaSvalue = ".$_POST["2"]."\n";
fwrite($handle,$data);
}
if($_POST["3"] != "1")
{
$data = "TimeShower:alphaSorder = ".$_POST["3"]."\n";
fwrite($handle,$data);
}
if($_POST["4"] != "1")
{
$data = "TimeShower:alphaEMorder = ".$_POST["4"]."\n";
fwrite($handle,$data);
}
if($_POST["5"] != "0.5")
{
$data = "TimeShower:pTmin = ".$_POST["5"]."\n";
fwrite($handle,$data);
}
if($_POST["6"] != "0.5")
{
$data = "TimeShower:pTminChgQ = ".$_POST["6"]."\n";
fwrite($handle,$data);
}
if($_POST["7"] != "0.0005")
{
$data = "TimeShower:pTminChgL = ".$_POST["7"]."\n";
fwrite($handle,$data);
}
if($_POST["8"] != "10.0")
{
$data = "TimeShower:mMaxGamma = ".$_POST["8"]."\n";
fwrite($handle,$data);
}
if($_POST["9"] != "on")
{
$data = "TimeShower:interleave = ".$_POST["9"]."\n";
fwrite($handle,$data);
}
if($_POST["10"] != "on")
{
$data = "TimeShower:allowBeamRecoil = ".$_POST["10"]."\n";
fwrite($handle,$data);
}
if($_POST["11"] != "1.")
{
$data = "TimeShower:octetOniumFraction = ".$_POST["11"]."\n";
fwrite($handle,$data);
}
if($_POST["12"] != "2.")
{
$data = "TimeShower:octetOniumColFac = ".$_POST["12"]."\n";
fwrite($handle,$data);
}
if($_POST["13"] != "on")
{
$data = "TimeShower:QCDshower = ".$_POST["13"]."\n";
fwrite($handle,$data);
}
if($_POST["14"] != "5")
{
$data = "TimeShower:nGluonToQuark = ".$_POST["14"]."\n";
fwrite($handle,$data);
}
if($_POST["15"] != "on")
{
$data = "TimeShower:QEDshowerByQ = ".$_POST["15"]."\n";
fwrite($handle,$data);
}
if($_POST["16"] != "on")
{
$data = "TimeShower:QEDshowerByL = ".$_POST["16"]."\n";
fwrite($handle,$data);
}
if($_POST["17"] != "on")
{
$data = "TimeShower:QEDshowerByGamma = ".$_POST["17"]."\n";
fwrite($handle,$data);
}
if($_POST["18"] != "5")
{
$data = "TimeShower:nGammaToQuark = ".$_POST["18"]."\n";
fwrite($handle,$data);
}
if($_POST["19"] != "3")
{
$data = "TimeShower:nGammaToLepton = ".$_POST["19"]."\n";
fwrite($handle,$data);
}
if($_POST["20"] != "on")
{
$data = "TimeShower:MEcorrections = ".$_POST["20"]."\n";
fwrite($handle,$data);
}
if($_POST["21"] != "on")
{
$data = "TimeShower:phiPolAsym = ".$_POST["21"]."\n";
fwrite($handle,$data);
}
fclose($handle);
}

?>
</body>
</html>

<!-- Copyright (C) 2009 Torbjorn Sjostrand -->