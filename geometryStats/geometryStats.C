/**
 * \file
 * \author Dmitri Naumov (dmitri.naumov@ufz.de)
 *
 * \copyright 2014 -- Helmholtz Centre for Environmental Research - UFZ
 * distributed under the GNU GPL version 3. See the LICENSE file for details.
 *
 * Volume of the domain and surface area of all but inlet and outlet patches.
 */

#include "fvCFD.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

    #include "setRootCase.H"
    #include "createTime.H"

    instantList timeDirs = timeSelector::select0(runTime, args);

    #include "createMesh.H"

    runTime.setTime(timeDirs.last(), timeDirs.size()-1);

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << endl;

        {
            const surfaceScalarField& Sf = mesh.magSf();
            scalar surface = 0;

            const fvPatchList& patches = mesh.boundary();
            forAll(patches, patchI)
            {
                const fvPatch& p = patches[patchI];
                if (p.name() == "inlet" || p.name() == "outlet")
                    continue;

                surface += sum(Sf.boundaryField()[patchI]);
            }
            Info << "Surface area = " << surface << endl;
        }

        {
            scalar volume = 0;
            forAll (mesh.cells(), cellI)
            {
                volume += mesh.V()[cellI];
            }
            Info << "Domain volume = " << volume << endl;
        }

        Info<< endl;
    }

    return 0;
}


// ************************************************************************* //
