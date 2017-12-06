
#ifndef __MYOCTANT_H_
#define __MYOCTANT_H_

#include "MyEntityManager.h"




	//System Class
	class MyOctant
	{
		static Simplex::uint m_uOctantCount; //will store the number of octants instantiated
		static Simplex::uint m_uMaxLevel;//will store the maximum level an octant can go to
		static Simplex::uint m_uIdealEntityCount; //will tell how many ideal Entities this object will contain

		Simplex::uint m_uID = 0; //Will store the current ID for this octant
		Simplex::uint m_uLevel = 0; //Will store the current level of the octant
		Simplex::uint m_uChildren = 0;// Number of children on the octant (either 0 or 8)

		float m_fSize = 0.0f; //Size of the octant

		Simplex::MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
		Simplex::MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton

		Simplex::vector3 m_v3Center = Simplex::vector3(0.0f); //Will store the center point of the octant
		Simplex::vector3 m_v3Min = Simplex::vector3(0.0f); //Will store the minimum vector of the octant
		Simplex::vector3 m_v3Max = Simplex::vector3(0.0f); //Will store the maximum vector of the octant

		MyOctant* m_pParent = nullptr;// Will store the parent of current octant
		MyOctant* m_pChild[8];//Will store the children of the current octant

		std::vector<Simplex::uint> m_EntityList; //List of Entities under this octant (Index in Entity Manager)

		MyOctant* m_pRoot = nullptr;//Root octant
		std::vector<MyOctant*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

	public:
		/*
		USAGE: Constructor, will create an octant containing all MagnaEntities Instances in the Mesh
		manager	currently contains
		ARGUMENTS:
		- uint a_nMaxLevel = 2 -> Sets the maximum level of subdivision
		- uint nIdealEntityCount = 5 -> Sets the ideal level of objects per octant
		OUTPUT: class object
		*/
		MyOctant(Simplex::uint a_nMaxLevel = 2, Simplex::uint a_nIdealEntityCount = 5);
		/*
		USAGE: Constructor
		ARGUMENTS:
		- vector3 a_v3Center -> Center of the octant in global space
		- float a_fSize -> size of each side of the octant volume
		OUTPUT: class object
		*/
		MyOctant(Simplex::vector3 a_v3Center, float a_fSize);
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object to copy
		OUTPUT: class object instance
		*/
		MyOctant(MyOctant const& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object to copy
		OUTPUT: ---
		*/
		MyOctant& operator=(MyOctant const& other);
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~MyOctant(void);
		/*
		USAGE: Changes object contents for other object's
		ARGUMENTS:
		- MyOctant& other -> object to swap content from
		OUTPUT: ---
		*/
		void Swap(MyOctant& other);
		/*
		USAGE: Gets this octant's size
		ARGUMENTS: ---
		OUTPUT: size of octant
		*/
		float GetSize(void);
		/*
		USAGE: Gets the center of the octant in global scape
		ARGUMENTS: ---
		OUTPUT: Center of the octant in global space
		*/
		Simplex::vector3 GetCenterGlobal(void);
		/*
		USAGE: Gets the min corner of the octant in global space
		ARGUMENTS: ---
		OUTPUT: Minimum in global space
		*/
		Simplex::vector3 GetMinGlobal(void);
		/*
		USAGE: Gets the max corner of the octant in global space
		ARGUMENTS: ---
		OUTPUT: Maximum in global space
		*/
		Simplex::vector3 GetMaxGlobal(void);
		/*
		USAGE: Asks if there is a collision with the Entity specified by index from
		the Bounding Object Manager
		ARGUMENTS:
		- int a_uRBIndex -> Index of the Entity in the Entity Manager
		OUTPUT: check of the collision
		*/
		bool IsColliding(Simplex::uint a_uRBIndex);
		/*
		USAGE: Displays the MyOctant volume specified by index including the objects underneath
		ARGUMENTS:
		- uint a_nIndex -> MyOctant to be displayed.
		- vector3 a_v3Color = REYELLOW -> Color of the volume to display.
		OUTPUT: ---
		*/
		void Display(Simplex::uint a_nIndex, Simplex::vector3 a_v3Color = Simplex::C_YELLOW);
		/*
		USAGE: Displays the MyOctant volume in the color specified
		ARGUMENTS:
		- vector3 a_v3Color = REYELLOW -> Color of the volume to display.
		OUTPUT: ---
		*/
		void Display(Simplex::vector3 a_v3Color = Simplex::C_YELLOW);
		/*
		USAGE: Displays the non empty leafs in the octree
		ARGUMENTS:
		- vector3 a_v3Color = REYELLOW -> Color of the volume to display.
		OUTPUT: ---
		*/
		void DisplayLeafs(Simplex::vector3 a_v3Color = Simplex::C_YELLOW);
		/*
		USAGE: Clears the Entity list for each node
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ClearEntityList(void);
		/*
		USAGE: allocates 8 smaller octants in the child pointers
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Subdivide(void);
		/*
		USAGE: returns the child specified in the index
		ARGUMENTS: uint a_nChild -> index of the child (from 0 to 7)
		OUTPUT: MyOctant object (child in index)
		*/
		MyOctant* GetChild(Simplex::uint a_nChild);
		/*
		USAGE: returns the parent of the octant
		ARGUMENTS: ---
		OUTPUT: MyOctant object (parent)
		*/
		MyOctant* GetParent(void);
		/*
		USAGE: Asks the MyOctant if it does not contain any children (its a leaf)
		ARGUMENTS: ---
		OUTPUT: It contains no children
		*/
		bool IsLeaf(void);
		/*
		USAGE: Asks the MyOctant if it contains more than this many Bounding Objects
		ARGUMENTS:
		- uint a_nEntities -> Number of Entities to query
		OUTPUT: It contains at least this many Entities
		*/
		bool ContainsMoreThan(Simplex::uint a_nEntities);
		/*
		USAGE: Deletes all children and the children of their children (almost sounds apocalyptic)
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void KillBranches(void);
		/*
		USAGE: Creates a tree using subdivisions, the max number of objects and levels
		ARGUMENTS:
		- uint a_nMaxLevel = 3 -> Sets the maximum level of the tree while constructing it
		OUTPUT: ---
		*/
		void ConstructTree(Simplex::uint a_nMaxLevel = 3);
		/*
		USAGE: Traverse the tree up to the leafs and sets the objects in them to the index
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void AssignIDtoEntity(void);

		/*
		USAGE: Gets the total number of octants in the world
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		Simplex::uint GetOctantCount(void);

	private:
		/*
		USAGE: Deallocates member fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Release(void);
		/*
		USAGE: Allocates member fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Init(void);
		/*
		USAGE: creates the list of all leafs that contains objects.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ConstructList(void);
	};//class


#endif //__OCTANTCLASS_H_

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */