#include "WeaponBase.h"

namespace dionite::combat {

WeaponBase WeaponSystem::fromTemplate(const std::string& id) {
    WeaponBase w;
    w.id = id;
    w.magazineSize = 12; w.reloadTime = 1.6f; w.projColor = "#fbbf24";
    if (id == "pistol")    { w.displayName="Soulpiercer Pistol"; w.kind=WeaponKind::Pistol;  w.damage=12; w.fireRate=0.2f;  w.projectileSpeed=540; w.spread=0.04f; w.magazineSize=12; }
    else if (id=="smg")    { w.displayName="Whisper SMG";        w.kind=WeaponKind::SMG;     w.damage=6;  w.fireRate=0.08f; w.projectileSpeed=600; w.spread=0.12f; w.magazineSize=40; w.projColor="#60a5fa"; }
    else if (id=="shotgun"){ w.displayName="Hex Scattergun";     w.kind=WeaponKind::Shotgun; w.damage=5;  w.pellets=7; w.fireRate=0.55f;w.projectileSpeed=520; w.spread=0.35f; w.magazineSize=6; w.projColor="#f97316"; }
    else if (id=="rifle")  { w.displayName="Stormcaller Rifle";  w.kind=WeaponKind::Rifle;   w.damage=28; w.fireRate=0.5f;  w.projectileSpeed=800; w.spread=0.01f; w.magazineSize=10; w.projColor="#a855f7"; }
    else if (id=="launcher"){w.displayName="Vesper Launcher";    w.kind=WeaponKind::Launcher;w.damage=65; w.fireRate=1.0f;  w.projectileSpeed=380; w.spread=0.02f; w.magazineSize=4;  w.explodeDmg=80; w.explodeRadius=90; w.projColor="#e11d48"; }
    else if (id=="staff")  { w.displayName="Ember Staff";        w.kind=WeaponKind::Staff;   w.damage=18; w.fireRate=0.32f; w.projectileSpeed=460; w.spread=0.02f; w.magazineSize=20; w.burnDmg=5; w.projColor="#fbbf24"; }
    else if (id=="frostwand"){w.displayName="Frostwand of Vael"; w.kind=WeaponKind::Wand;    w.damage=14; w.fireRate=0.22f; w.projectileSpeed=520; w.spread=0.03f; w.magazineSize=24; w.slow=0.4f; w.projColor="#38bdf8"; }
    w.currentMagazine = w.magazineSize;
    return w;
}

} // namespace dionite::combat
