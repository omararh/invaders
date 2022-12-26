#include <list>
#include <string>
#include <iostream>

class position {
	public:
		position(unsigned int x, unsigned int y):_x(x),_y(y) {}
		unsigned int x() const { return _x; }
		unsigned int y() const { return _y; }
		void setx(unsigned int x) { _x=x; }
		void sety(unsigned int y) { _y=y; }
		bool operator==(position const & p2) const { return (_x==p2._x) && (_y==p2._y); }
		bool operator!=(position const & p2) const { return !operator==(p2); }
		std::string tostring() const { return std::string("(") + std::to_string(_x)+","+std::to_string(_y)+")"; }
	private:
		unsigned int _x;
		unsigned int _y;
};
std::ostream & operator<<(std::ostream & os, position const & p);

class taille {
	public:
		taille(unsigned int w, unsigned int h):_w(w),_h(h) {}
		unsigned int w() const { return _w; }
		unsigned int h() const { return _h; }
		void setw(unsigned int w) { _w = w; }
	private:
		unsigned int _w;
		unsigned int _h;
};

enum class direction {
	stop,
	droite,
	gauche
};

class objet {
	public:
		objet(position const & p):_pos(p) {}
		virtual ~objet() =default;
		position const & pos() const { return _pos; }
		void setpos(position const & pos) { _pos = pos; }
		virtual taille tai() const =0;
		virtual void appliquerdeplacement() =0;
		bool contientposition(position const & p) const;
		void sortieflux(std::ostream & os) const;
	protected:
		virtual char typeobjet() const=0;
	private:
		position _pos;
};

class projectile: public objet {
	public:
		explicit projectile(position const & p):objet(p) {}
		taille tai() const override { return taille(1,1); }
		void appliquerdeplacement() override;
	protected:
		char typeobjet() const override { return 'P'; }
};

class objetactif: public objet {
	public:
		objetactif(position const & p, unsigned int xmin, unsigned int xmax, direction d): objet(p), _xmin(xmin), _xmax(xmax), _dir(d) {}
		unsigned int xmin() const { return _xmin; }
		unsigned int xmax() const { return _xmax; }
		direction dir() const { return _dir; }
		void setdir(direction d) { _dir = d; }
		void appliquerdeplacement() override;
	private:
		unsigned int _xmin;
		unsigned int _xmax;
		direction _dir;
};

class invader: public objetactif {
	public:
		invader(position const & p, unsigned int xmin, unsigned int xmax, direction d): objetactif(p,xmin,xmax,d) {}
		taille tai() const override { return taille(11,8); }
		void appliquerdeplacement() override;
	protected:
		char typeobjet() const override { return 'I'; }
};

class joueur: public objetactif {
	public:
		joueur(position const & p, taille const & t, unsigned int xmin, unsigned int xmax, direction d): objetactif(p,xmin,xmax,d), _tai(t) {}
		taille tai() const override { return _tai; }
		void malus() { _tai.setw(std::min(_tai.w()*2,100U)); }
		void bonus() { _tai.setw(std::max(_tai.w()/2,3U)); }
	protected:
		char typeobjet() const override { return 'J'; }
	private:
		taille _tai;
};

class jeu {
	public:
		jeu(): _joueur(position(320/2-9/2,180), taille(9,8), 10, 310, direction::stop) {}
		bool ajouter(projectile const & p);
		void ajouter(invader const & i);

		void directionjoueur(direction d) { _joueur.setdir(d); }
		void tirjoueur();
		void tourdejeu();
		void afficher(std::ostream & os) const;

		// Non demandé dans le sujet, mais nécessaire pour l'affichage dans sfml
		class joueur const & joueur() const { return _joueur; }
		std::list<invader> invaders() const { return _inva; }
		std::list<projectile> projectiles() const { return _projs; }
	private:
		void projectiledisparition();
		void projectilecollision();
	private:
		std::list<projectile> _projs;
		std::list<invader> _inva;
		class joueur _joueur;
};


