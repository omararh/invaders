#include "invaders.hh"
#include <algorithm>

std::ostream & operator<<(std::ostream & os, position const & p) {
	os << p.tostring();
	return os;
}

bool objet::contientposition(position const & p) const {
	return (p.x()>=pos().x()) && (p.x()<(pos().x()+tai().w()))
		&& (p.y()>=pos().y()) && (p.y()<(pos().y()+tai().h()));
}

void objet::sortieflux(std::ostream & os) const {
	os << typeobjet() << _pos.tostring();
}

void projectile::appliquerdeplacement() {
	if (pos().y() != 0)
		setpos(position(pos().x(), pos().y()-1));
}

void objetactif::appliquerdeplacement() {
	switch (_dir) {
		case direction::stop: break;
		case direction::droite:
			if (pos().x()+tai().w() < _xmax)
				setpos(position(pos().x()+1, pos().y()));
			break;
		case direction::gauche:
			if (pos().x() > _xmin)
				setpos(position(pos().x()-1, pos().y()));
			break;
	}
}

void invader::appliquerdeplacement() {
	if ((dir() == direction::gauche) && (pos().x() == xmin())) {
		setdir(direction::droite);
		setpos(position(pos().x(), pos().y()+1));
	} else if ((dir() == direction::droite) && (pos().x()+tai().w() == xmax())) {
		setdir(direction::gauche);
		setpos(position(pos().x(), pos().y()+1));
	}
	else
		objetactif::appliquerdeplacement();
}


bool jeu::ajouter(projectile const & p) {
	for (auto const & i : _projs)
		if (p.pos() == i.pos())
			return false;
	_projs.push_back(p);
	return true;
}

void jeu::ajouter(invader const & i) {
	_inva.push_back(i);
}

void jeu::tirjoueur() {
	ajouter(projectile(position(_joueur.pos().x()+_joueur.tai().w()/2, _joueur.pos().y()-1)));
}

void jeu::tourdejeu() {
	for (auto & i : _projs)
		i.appliquerdeplacement();
	for (auto & i : _inva)
		i.appliquerdeplacement();
	projectiledisparition();
	projectilecollision();
	_joueur.appliquerdeplacement();
}

void jeu::afficher(std::ostream & os) const {
	_joueur.sortieflux(os);
	for (auto & i : _projs)
		i.sortieflux(os);
	for (auto & i : _inva)
		i.sortieflux(os);
}

void jeu::projectiledisparition() {
	auto itp(_projs.begin());
	while (itp != _projs.end()) {
		if (itp->pos().y() == 0)
			itp = _projs.erase(itp);
		else
			++itp;
	}
}

void jeu::projectilecollision() {
	auto itp(_projs.begin());
	while (itp != _projs.end()) {
		auto iti(_inva.begin());
		bool boom(false);
		while (!boom && (iti != _inva.end()))
			if (iti->contientposition(itp->pos())) {
				boom = true;
				_inva.erase(iti);
			}
			else
				++iti;
		if (boom)
			itp = _projs.erase(itp);
		else
			++itp;
	}
}
